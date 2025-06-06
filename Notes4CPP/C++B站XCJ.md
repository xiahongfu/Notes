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