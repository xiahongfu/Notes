TODO：
shot2see最大的困难：1）录像流异步停止（多下一些buffer，为什么要异步停止？？？）导致，2）metaBuffer先到，ImageBuffer后到，不能直接标记停止，需要等ImageBuffer到了之后再标记。
多摄优化可以扯的点：
    * fromsensor短时delay：需要考虑多线程竞争-->加锁、线程同步；
    * 多摄按需boost：提频boost begin--boost end怎么成对===》。 1.首先需要分析哪些代码段需要提频（在代码段中加打点，统计执行时间、CPU执行时间等信息）2.用RAII进行管理，实现了一个类+宏，构造时调用begin，析构时调用end
    * 可以把解得那个问题单扯到这上面来。
性能白盒化可以扯的点：
UTU插件：UnrealCSharp的技术原理，C++和CS怎么互相调用起来的？反射在哪里用上的？用上了注解，干啥用的？
鸿蒙化可以扯的点：传输工具。



HAL框架
需要总结一下HAL层的进程间是怎么通信的
HAL侧的buffer流转
enable_shared_from_this的用例
用到的各种设计模式
