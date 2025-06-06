```c++
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