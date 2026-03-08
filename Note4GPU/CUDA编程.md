**官方文档**
cuda官方教程：https://docs.nvidia.com/cuda/cuda-programming-guide/
CUDA C Programming Guide 的中文翻译版。中文翻译版https://github.com/HeKun-NVIDIA/CUDA-Programming-Guide-in-Chinese
CUDA Toolkit Documentation 13.1：https://docs.nvidia.cn/cuda/index.html

**好文**
CUDA编程基础(四)—GPU线程调度：https://zhuanlan.zhihu.com/p/12083951223
面试常见手撕CUDA算子实现及优化：https://zhuanlan.zhihu.com/p/1904550020918801886
CUDA算子手撕与面试：https://zhuanlan.zhihu.com/p/12661298743
【CUDA编程】束内洗牌函数：https://zhuanlan.zhihu.com/p/669957986
CUDA编程入门极简教程：https://zhuanlan.zhihu.com/p/34587739

# GPU Hardware Model
GPC：Graphics Processing Cluster
SM：Streaming Multiprocessor


a unified data cache: L1 cache and shared memory

**SM结构**
SM=SM=a local register file + a unified data cache(L1 cache and shared memory) + functional units(core/SFU等)
CUDA core：包含单精度浮点数处理单元，执行基本的运算
Shared Memrory: 可编程，由同一个ThreadBlock间的线程共享
L1 Cache: 不可编程。物理上Shared Memrory和L1 Cache是共享的，可以通过API控制两者配比
Register File: 存放指令操作数；也有一些特殊寄存器存放系统变量（如grid维度，threadblock维度，线程id等）
LD/ST

内存模型：L2缓存+显存是全局共享的，L1缓存和SharedMemory是SM内部共享的，
线程模型：一个ThreadBlock的所有线程都在同一个SM中执行。一个SM可以持有多个ThreadBlock。CUDA编程模型要求：不同ThreadBlock的线程不要存在数据依赖
Grid -> ThreadBlock -> wrap=32*Thread   GPU不会单个线程调度，而是按wrap调度。
Grid：一个kernel启动的所有线程称为一个Grid
一个warp在同一个周期只能执行相同的指令。所以分支函数会导致性能下降。

一个SM最多32个Block。





**kernel函数**
* `__global__`: GPU执行，CPU/GPU调用。即kernel函数。调用时必须用<<<>>>指定launch的参数
* `__device__`: GPU执行，GPU调用
* `__host__`: CPU执行，CPU调用。没有任何修饰的函数默认是host code

`__device__`和`__host__`可同时使用，标识一个函数可以在任意设备上执行，任意设备上调用。但是此时threadIdx，blockIdx是用不了的。


**内置变量**
* **threadIdx**: uint3类型。当前线程在所属block中的坐标。有xyz三个分量。
* **blockIdx**: uint3类型。当前线程的block 在所属grid中的坐标，包含xyz三个分量。
* **blockDim**: dim3类型。block的形状，包含xyz三个分量分别表示 宽高厚
* **gridDim**: dim3类型。grid的形状。


**shared memory**
__shared__ 用来声明共享内存
__syncthreads()：barrier 同步原语。线程执行到__syncthreads()之后，必须等待所有线程都执行到__syncthreads()，才继续往下执行。只对同一个线程块的线程起作用

【CUDA编程概念】一、什么是bank conflict？：https://zhuanlan.zhihu.com/p/659142274
Bank：shared memory会把内存划分为一个个的小格子，每个格子大小为4B/8B，这一个个的小格子按顺序编号从0~31。编号相同的所有小格子组成一个Bank，共有32个Bank。每个Bank可能会有多个格子。

当**同一个warp中的多个线程访问同一个Bank中的不同地址的数据**的时候，会造成Bank confilt，退化成串行执行。以下两种特殊情况不属于warp confilt
* 同一个warp的多个线程读同一个bank的同一个地址：一个线程读，然后广播到其他线程。
* 同一个warp的多个线程写同一个bank的同一个地址：仅一个线程写。哪个线程写是未定义的。
* 不同warp不会conflict
