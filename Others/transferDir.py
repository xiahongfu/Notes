import subprocess
import sys
import os
import re
import uuid
import time
from concurrent.futures import ThreadPoolExecutor
import threading

class AtomicInteger:
    def __init__(self, initial=0):
        self.value = initial
        self.lock = threading.Lock()

    def get(self):
        with self.lock:
            return self.value

    def set(self, new_value):
        with self.lock:
            self.value = new_value

    def increment(self):
        with self.lock:
            self.value += 1
            return self.value
    
    def decrement(self):
        with self.lock:
            self.value -= 1
            return self.value
            

def parseArgs():
    import argparse
    parser = argparse.ArgumentParser(description='Put all the files in source_dir and source_dir itself into the target_dir.')
    parser.add_argument('-s', '--source', help='source dir or source file.')
    parser.add_argument('-t', '--target', help='target dir.')
    parser.add_argument('-b', '--bundle_name', help='Bundle name, use for start bftpd service', default="com.example.myapplication")
    parser.add_argument('-a', '--ability', help='EntryAbility, use for start bfptd service', default="TuanjiePlayerAbility")
    parser.add_argument('-p', '--port', help='bftpd service will be run on this port, use for start bftpd service. Default is 56980', default=56980)
    parser.add_argument('-tc', '--thread_count', help='the count of thread. Default is 1.', default=1)

    args = parser.parse_args()
    source_dir = args.source
    target_dir = args.target
    if source_dir is None or target_dir is None:
        print("未指定source_dir和target_dir，请使用--help查看文档")
        sys.exit(-1)
    if not os.path.exists(source_dir):
        print(f"{source_dir}不存在")
        sys.exit(-1)
    isDir = os.path.isdir(source_dir)
    if source_dir.endswith("/") or source_dir.endswith("\\"):
        source_dir = source_dir[:-1]

    bundle_name = args.bundle_name
    ability = args.ability
    port = int(args.port)
    thread_count = int(args.thread_count)
    if (not isDir and thread_count is not None):
        thread_count = 1
        print("正在传输文件，忽略thread_count字段。")
    if (thread_count <= 0):
        print("thread count should greater than zero.")
        sys.exit(-1)
    return source_dir, target_dir, bundle_name, ability, port, isDir, thread_count

source_dir, target_dir, bundle_name, ability, port, isDir, thread_count = parseArgs()


class BftpdService:
    newPort = port + thread_count
    defeatTime = 0
    MAX_DEFEAT_TIME = 16
    @staticmethod
    def getBftpdProcess():  # 获取bftpd所有进程
        result = subprocess.run(f"hdc shell \"ps -ef | grep bftpd\"", stdout=subprocess.PIPE, shell=True) # ["hdc", "shell", "ps", "-ef"]
        output = result.stdout.decode('utf-8')
        lines = output.split('\r\n')
        lines = [line for line in lines if "grep" not in line]
        return lines
    
    @staticmethod
    def killBftpdProcess():  # 杀死所有bftpd进程。仅root手机可用。
        lines = BftpdService.getBftpdProcess()
        for line in lines:
            match = re.search(r'\s(\d+)\s+', line)
            thread_id = match.group(1) if match else -1
            if (thread_id != -1):
                subprocess.run(f"hdc shell kill {thread_id}", stdout=subprocess.PIPE, shell=True)

    @staticmethod
    def startBftpd(port=port, isPrint=True):
        result = subprocess.run(["hdc", "shell", "aa", "process", "-b", bundle_name, "-a", ability, "-p", "/system/bin/bftpd -D -p " + str(port), "-S"], stdout=subprocess.PIPE)
        output = result.stdout.decode('utf-8')
        if isPrint:
            print(output)

    @staticmethod
    def runBftpd(bundle_name=bundle_name, ability=ability, port=port, bftpdCnt = thread_count, kill=False):
        """
        port: 在 [port ~ port+bftpdCnt-1] 端口上启用bftpd服务进行监听。
        bftpdCnt: 想要开启的bftpd服务数量。
        kill: 是否杀死现有进程，默认为False，非root手机不可用。
        """
        if (bundle_name is None or ability is None or port is None):
            print("bftpd启动失败，请使用-b指定bundle_name，使用-a指定Entry Ability，使用-p指定端口（端口默认为56980）")
            sys.exit(-1)

        if kill:
            BftpdService.killBftpdProcess()  # 这里无root权限没法kill。暂时不考虑kill的逻辑了。
        for i in range(bftpdCnt):
            BftpdService.startBftpd(int(port)+i)
        running = len(BftpdService.getBftpdProcess()) > 0
        if not running:
            print("bftpd服务启动失败，请检查手机是否打开USB调试权限或应用是否具有INTERNET权限")
            sys.exit(-1)
        # else:
        #     # print("bftpd已启动成功")
        #     pass
    
    @staticmethod
    def runNewBftpd():  # 用于在运行过程中动态开启bftpd服务。降低卡死概率。
        # 每卡死 MAX_DEFEAT_TIME 次会在下一个端口开启bftpd服务，以降低卡死概率。
        if (BftpdService.defeatTime > BftpdService.MAX_DEFEAT_TIME):
            BftpdService.defeatTime = 0
            BftpdService.newPort += 1
            BftpdService.startBftpd(port=BftpdService.newPort, isPrint=False)
        else:
            BftpdService.defeatTime += 1


def rmDir(tmpRoot):
    command = f"hdc shell rm -rf '{tmpRoot}'"
    os.system(command)


class DirUtils:
    MAX_RETRY = 20
    dircnt = AtomicInteger(0)
    filecnt = AtomicInteger(0)
    loadbalance = 0  # 用于判断由哪个线程执行文件复制操作。
    
    @staticmethod
    def getRativePath(rootPath, path): 
        # 获取相对路径。比如rootPath="/test", path="/test/dir1/dir2"。那么返回值就是dir1/dir2。返回值是相对路径，不能以/开头和结尾
        rpath = path
        if (path.startswith(rootPath)):
            rpath = rpath[len(rootPath):]
        if (rpath.startswith("/") or rpath.startswith("\\")):
            rpath = rpath[1:]
        if (rpath.endswith("/") or rpath.endswith("\\")):
            rpath = rpath[:-1]
        return rpath
    
    @staticmethod
    def _listDir(_path):
        command = f"hdc shell ls {_path} -F"
        result = subprocess.run(command, stdout=subprocess.PIPE, shell=True).stdout.decode('utf-8').split("\r\n")[:-1]
        result = [x[:-1] for x in result if x[-1]=='/']
        return result
    
    @staticmethod
    def _list(_path):
        dir_list = DirUtils._listDir(_path)
        command = f"hdc shell ls '{_path}'"
        result = subprocess.run(command, stdout=subprocess.PIPE, shell=True).stdout.decode('utf-8')
        output = result.split("\r\n")[:-1]
        file_list = [x for x in output if x not in dir_list]
        return file_list, dir_list
    
    @staticmethod
    def _move(_source, _target, port=port, depth=0, isprint=False):
        if (depth >= DirUtils.MAX_RETRY):
            print(f"超过最大重试次数，传输失败{_target}")
            return False
        command = f"hdc shell \"ftpget -p {str(port)} -P guest -u anonymous localhost -s '{_source}' '{_target}'\""
        result = subprocess.run(command, stdout=subprocess.PIPE, shell=True)
        result = result.stdout.decode('utf-8')
        if ("" != result):
            if ("ftpget: connect: Connection refused\r\n" == result):
                if isprint:
                    print("\nbftpd服务停止运行，正在重新启动")
                return DirUtils._move(_source, _target, port=port, depth=depth+1, isprint=isprint)
            elif (not result.endswith("ftpget: 451 Error: File exists.\r\n")):
                if isprint:
                    print(f"\n 复制{_target}遇见未知错误：" + result)
                return DirUtils._move(_source, _target, port=port, depth=depth+1, isprint=isprint)
        return True
    
    @staticmethod
    def _mkdir(_path, port=port, depth=0, isprint=False):
        if (depth >= DirUtils.MAX_RETRY):
            if isprint:
                print(f"超过最大重试次数，传输失败{_path}")
            return False
        command = f"hdc shell \"ftpget -p {str(port)} -P guest -u anonymous localhost -M '{_path}'\""
        result = subprocess.run(command, stdout=subprocess.PIPE, shell=True)
        result = result.stdout.decode('utf-8')
        if ("" != result):
            if ("ftpget: connect: Connection refused\r\n" == result):
                if isprint:
                    print("bftpd服务停止运行，正在重新启动")
                return DirUtils._mkdir(_path, depth=depth+1, isprint=isprint)
            elif (not result.endswith("ftpget: 451 Error: File exists.\r\n")):
                if isprint:
                    print(f"\n 创建{_path}遇见未知错误：" + result)
                return DirUtils._mkdir(_path, depth=depth+1, isprint=isprint)
        return True
                
    @staticmethod
    def move2target(source, target='/data/sotrage/el1/database', port=port):
        source = source.replace("\\", "/")
        def _walk(_path):
            files_list, dir_list = DirUtils._list(_path)
            for file in files_list:
                DirUtils.loadbalance+=1
                source_path = os.path.join(_path, file).replace("\\", "/")
                target_path = os.path.join(target, os.path.basename(source), DirUtils.getRativePath(source, _path), file).replace("\\", "/")
                if (thread_count == 1):
                    if DirUtils._move(source_path, target_path, port):
                        print("\r进度: 文件夹{}，文件{}".format(DirUtils.dircnt.get(), DirUtils.filecnt.increment()), end="")
                else:
                    future = pool[DirUtils.loadbalance%(len(pool)-1)+1].submit(DirUtils._move, source_path, target_path, int(port)+1+DirUtils.loadbalance%(len(pool)-1))
                    future.add_done_callback(_whenfiledone)
            del files_list

            for dir in dir_list:
                source_path = os.path.join(_path, dir).replace("\\", "/")
                target_path = os.path.join(target, os.path.basename(source), DirUtils.getRativePath(source, _path), dir).replace("\\", "/")
                # 创建文件夹
                tgtidx = DirUtils.dircnt.get() + 1
                future = pool[0].submit(DirUtils._mkdir, target_path, BftpdService.newPort)
                future.add_done_callback(_whendirdone)
                # 等待进度+1，如果迟迟等不来，则shutdown pool[0]
                while not _waitdirdone(tgtidx=tgtidx):
                    # print(target_path)
                    BftpdService.runNewBftpd()
                    pool[0].shutdown(wait=False)
                    pool[0] = ThreadPoolExecutor(max_workers=1)
                    future = pool[0].submit(DirUtils._mkdir, target_path, BftpdService.newPort)
                    # future = pool[0].submit(DirUtils._mkdir, target_path)
                    future.add_done_callback(_whendirdone)
                _walk(source_path)
            del dir_list

        def _whendirdone(_future):
            if _future.result():
                print("\r进度: 文件夹{}，文件{}".format(DirUtils.dircnt.increment(), DirUtils.filecnt.get()), end="")

        def _whenfiledone(_future):
            if _future.result():
                print("\r进度: 文件夹{}，文件{}".format(DirUtils.dircnt.get(), DirUtils.filecnt.increment()), end="")

        def _waitdirdone(tgtidx):
            sleeptime = 0.0
            while True:
                if (sleeptime > 2.1):
                    return False
                if (DirUtils.dircnt.get() < tgtidx):
                    time.sleep(0.05)
                    sleeptime += 0.05
                else:
                    return True
        
        t = time.time()
        if not DirUtils._mkdir(os.path.join(target, os.path.basename(source)).replace("\\", "/"), isprint=True):
            print("创建根目录失败。请检查各个字段是否填写正确。")
            return False
        pool = [ThreadPoolExecutor(max_workers=1) for i in range(thread_count)]
        _walk(source)
        for i in range(thread_count):
            pool[i].shutdown(wait=True)
        print(f'复制耗时: {time.time() - t:.8f}s')

    # 将fromDir目录拷贝到tmpDir下。直接传输文件夹
    @staticmethod
    def copy2tmp(fromDir): 
        t = time.time()
        fromDir = os.path.abspath(fromDir)  # 获取绝对路径
        s_uuid=''.join(str(uuid.uuid4()).split('-'))
        tmpDir = "/data/local/tmp/" + s_uuid
        os.system(f"hdc shell mkdir {tmpDir}")
        print("临时文件夹：" + tmpDir)
        os.system(f"hdc file send \"{fromDir}\" {tmpDir}")
        rootDir = os.path.basename(fromDir)
        print(f'传输到tmp文件夹下时间: {time.time() - t:.8f}s')
        return tmpDir, rootDir



def transfer_main(rmAfterFinished=False, tmpRoot=None, rootDir=None):
    """
    rmAfterFinished: 在完成之后删除tmp目录下的文件夹。默认为False
    tmpRoot: tmp目录下的路径，形式为 /data/local/tmp/a3f6ab02e7444960b242e0a83ec9f15a ，一般不用自己指定。
    rootDir: 文件名或文件夹名。代表的是你希望传输到沙箱路径的文件/文件夹，一般不用自己指定。
    """
    if tmpRoot is None or rootDir is None:
        tmpRoot, rootDir = DirUtils.copy2tmp(source_dir)

    BftpdService.runBftpd(bundle_name=bundle_name, ability=ability, port=port, kill=False)  # 开启bftpd服务。

    if isDir:
        DirUtils.move2target(os.path.join(tmpRoot, rootDir), target_dir, port=port)
    else:
        _source = os.path.join(tmpRoot, rootDir).replace("\\", "/")
        _target = os.path.join(target_dir, rootDir).replace("\\", "/")
        DirUtils._move(_source, _target, depth=0)

    if rmAfterFinished:
        rmDir(tmpRoot=tmpRoot)



# transfer_main(rmAfterFinished=False, tmpRoot="/data/local/tmp/a3f6ab02e7444960b242e0a83ec9f15a", rootDir="yourDirectory")  # 如果在tmp目录下已经有了，只需要指定tmpRoot和rootDir即可
transfer_main()
