仓库地址：[U1 · AtomGit\_开放原子开源基金会代码托管平台](https://atomgit.com/191cb5c1bf0f11ed851dd11cfc70da42/U1)
需要先创建一个账号，然后把邮箱发给曾聪让他给你开权限，然后就可以下载项目了。
绿区下载需要把网关断掉。

## 通过git下载

网络不好，直接下载zip会经常断连，所以推荐使用git下载

* 使用git生成秘钥（如果已经生成则可以直接复用）。生成的秘钥在~\.ssh目录下。

```shell
cd ~/.ssh
cat ./id_rsa.pub  # 复制这条命令的输出
```

* 配置ssh公钥，在 `个人设置--安全中心--SSH Keys`中。
* 如果一切配置成功，则可以执行`git clone git@atomgit.com:191cb5c1bf0f11ed851dd11cfc70da42/U1.git`。这里需要断开xgate

## 编译

在git项目的readme里有具体步骤

* 先双击执行setup.bat。这里会安装依赖，需要断开xgate
* 再双击执行generateProjectFiles.bat。如果报错则是visual studio相关的依赖没有安装。按照以下步骤即可。亲测visual studio 2022上也可以安装并编译成功。需要安装：
  * **.NET 桌面开发** 并勾选 **.NET Framework4.6.2-4.7.1 开发工具**。
  * **使用C++的游戏开发** 并勾选 **Unreal Engine 安装程序**
* 再双击刚刚生成的UE4.sln用Visual Studio打开
* 找到Configure Manager，确保Set your solution configuration to **Development Editor** and your solution platform to **Win64**
* 在左侧Solution Explorer窗口右键Solution 'UE4'。点击Build Solution。
* 双击 D:\yourpath\U1\Engine\Binaries\Win64\UE4Editor.exe

## 出包

* 点击 `文件->打包项目->打包设置->OpenHarmony SDK` 完成OpenHarmony SDK的配置，需要设置：
  * OpenHarmony SDK地址：`D:/DevEco Studio/devecostudio-503/sdk/HarmonyOS-NEXT-DB6`
  * SDK中包含的native地址，形式通常是[SDK]/openharmony/native：`D:/DevEco Studio/devecostudio-503/sdk/HarmonyOS-NEXT-DB6/openharmony/native`
  * Deveco安装地址：`D:/DevEco Studio/devecostudio-503`
* 点击 `文件->打包项目->OpenHarmony->OpenHarmony(ETC2)`进行出包
* 包会出到工程的Intermediate目录下的OpenHarmony下。用DevEco打开，然后签名安装即可。
