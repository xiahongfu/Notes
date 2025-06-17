# sec3
3.20
[switch语句编译期间会做优化](https://zhuanlan.zhihu.com/p/519272545)
* 如果case的范围合适的话，就会生成一个table，case 的值就是下标。
* 如果case的范围不合适，且开了编译优化，那么也有可能使用二分查找的方式来匹配对应的下标。
实际上每个case都是一个程序段，运行时是跳转到对应的代码片段往下执行。如果不写break，那就会依次执行后面所有的代码段，直到执行完为止，写了break相当于在break处增加一条jmp汇编语句，用于退出整个Switch。


# sec4

## 数组

栈区数组：`int arr[4]{0};`  大小只能是编译时常量（constexpr、字面量、`const int c = 0;`）。因为编译器在编译时就需要确定栈区数组的空间大小。

```c++
// 栈区数组的初始化
int arr0[10]{0};
int arr1[10] = {0};
int arr2[] = {1,2,3,4,5};
char arr3[] = "hello world"; // 字符串结尾会有个\0

// 堆区数组初始化。基本不会使用堆区数组了，了解一下即可
int* arr1 = new int[4]{0};
int* arr2 = new int[4]{0,1,2,3}; // 这里的4不能省略
int size=10;
int* arr3 = new int[size]{0};
int temp[10]{10};
memcpy(arr3, temp, sizeof(temp))
delete[] arr1; arr1 = nullptr;
```


## Vector
```c++
find(vec.begin(), vec.end(), 5);  // 返回iterator
vector.size(); // 元素个数
vector.capacity();  // 容量
vector.reserve(8);  // 扩充内存空间。只扩充内存，准备好内存空间。
vector.resize(8);  // 增加元素数量。 会初始化对象。
vector.shrink_to_fit();  // 有多少元素就分配多少内存

vector.push_back("test");
/*
1. 将"test"转为string对象
2. vector内部分配string对象空间
3. 复制一份string存入vector    早期C++，相当于有两个对象，两份堆空间
3. 现代版本移动一份存入vector   现代C++，相当于有两个对象，一份堆空间
*/

vector.emplace_back("test");
/*
1. vector内部分配string的对象空间
2. 直接将string对象创建在分配的空间中。
*/
```


# Sec 5

## 函数变量与作用域分析
全局变量在进入main函数前申请空间，C++开发的程序不应该用全局变量。
静态全局变量在进入main函数前申请空间，只有当前cpp能访问静态全局变量。
局部静态变量，静态变量出栈之后不会销毁，作用域仅限于函数内部。生命周期是第一次运行此行代码申请空间，main函数结束后销毁。C++11之前的局部静态变量是线程不安全的。C++11之后的是线程安全的。

## 指针和引用
引用从语义上说就是变量的别名。实际上是const T*（存疑）。引用的好处是有更强的类型检查，语法层面上更清晰。在定义时有特定语法（T& a{b}），使用时和原变量一致。引用在定义时必须初始化。

返回值不能返回栈区的指针和引用。

## 函数与数组
```C++
void TestSizeofArr(int outerArr[10])
{
    int innerArr[10] {1,2,3,4,5};
    cout<<sizeof(outerArr)<<endl;  // 8  经过传参，数组变成指针。
    cout<<sizeof(innerArr)<<endl;  // 40 
} 

using ArrType=int[10];
ArrType& TestSizeofRef(int (&arr)[10])  // 返回引用数组
{
    cout<<sizeof(arr)<<endl;  // 40 传递引用，可以获取到数组大小。
    return arr;
}
```

## 多文件编译与函数
.h/.hpp
* 函数声明：如果函数有默认参数，只在声明的时候加即可。函数声明前加extern "C"代表这是一个C语言函数（extern "C" {}即可声明多个C语言函数，在大括号里声明即可）。
* 全局变量声明：不建议使用。语法：extern int a;  在.cpp中做定义。
* 类声明
声明是不用内存的，实现才占用内存空间。

.cpp/.cc
* 函数定义
* 全局变量定义
* 类定义

预处理宏
#pragma once   控制头文件在相同的.cpp文件中只被引用一次（可以被多个.cpp引用）。

# 面向对象
没有成员变量的类，它的对象占1个字节的内存空间

this 指针指向当前对象的地址。

## 内联函数

内联函数是带有inline标识的函数，另外类的成员函数中，函数的声明和定义放在一起的这些成员函数默认是内联的。内联函数的坏处是，如果函数实现发生变动，那么使用到这个函数的所有cpp文件都需要重新编译。而非内联函数则只要函数的定义没变，那么其他使用到这个函数的.cpp文件也不需要重新编译，可以加快编译速度。
普通函数调用的时候是读取函数的地址然后去访问，存在入栈、出栈的过程。但是内联函数会在编译期执行代码的复制和替换，相当于把代码复制过来（称之为**内联展开**）直接运行，加快函数执行速度。不过是否进行复制和替换，编译器是有最终决定权的，有可能因为函数过于复杂导致编译器拒绝内敛，仍然以普通的方式进行函数调用。
内联函数由于是直接拷贝代码到被调用处的，所以坏处是会增大编译后的二进制体积，好处是展开后的代码也可以参与编译器的局部优化，如常量传播、死代码消除等。

## 静态成员变量、静态成员函数
为什么静态成员变量不能在类内定义？因为声明不申请空间，定义申请空间。部分情况下静态成员常量可以在类内初始化，因为编译器会把它当成常量。

```C++
class Singleton
{
public:
    static Singleton& Instance()
    {
        static Singleton instance;
        return instance;
    }
private:
    Singleton();
};
```
```C++
class Test
{
public:
    // 静态成员函数可以给成员函数做一层包装。变成普通函数。
    static void RunTestFunc(Test& test)
    {
        test.TestFunc();
    }
private:
    void TestFunc()
    {
        cout << "TestFunc()" << endl;
    }
};
```

## 继承
继承存在成员变量同名、成员函数同名时  内存是怎么布局的。

## 多态

### 动态多态

虚函数


### 静态多态

函数重载、模板

# Sec 8
放弃了，没啥用