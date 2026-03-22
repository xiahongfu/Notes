# C++ 字符串相关 API 与常见用法（速查）

本文聚焦标准库（STL）里与“字符串”相关的类型、常用 API、典型写法与注意点。默认讨论的是窄字符（`char`）与 UTF-8 字节序列（注意：标准库对 Unicode 语义处理有限）。

---

## 1. 相关头文件一览

- `<string>`：`std::string`、数字转换（`stoi/stod/...`、`to_string`）
- `<string_view>`：`std::string_view`
- `<sstream>`：`std::istringstream/std::ostringstream/std::stringstream`
- `<iostream>`：`std::getline`、流输入输出
- `<fstream>`：`std::ifstream/std::ofstream/std::fstream`（文件流）
- `<iomanip>`：流格式控制（`std::setw/std::setprecision/...`）
- `<limits>`：配合 `ignore()` 丢弃剩余输入（`std::numeric_limits<>::max()`）
- `<charconv>`：`std::from_chars/std::to_chars`（更快、无异常）
- `<format>`（C++20）：`std::format`
- `<regex>`：正则
- `<cstring>`：C 字符串/字节操作（`strlen/strcmp/memcpy/...`）
- `<cctype>`：字符分类（`isspace/isdigit/tolower/...`）
- `<algorithm>`：`find/search/replace/remove_if/reverse/...`
- `<ranges>`（C++20，可选）：ranges 算法 + 视图

---

## 2. 字符串类型全家桶

### 2.1 `std::string`

- 拥有内存（owning），可修改，动态扩容。
- 常用于：存储、拼接、修改、作为 API 的所有权载体。

### 2.2 `std::string_view`

- 非拥有（non-owning）视图，本质是“指针 + 长度”。
- 常用于：只读参数传递、避免拷贝、对子串做切片。

⚠️ 注意：`string_view` **不延长**底层字符的生命周期。

```cpp
std::string_view sv;
{
    std::string s = "abc";
    sv = s;              // OK：此时 sv 引用 s 的内容
}                         // s 析构
// sv 现在悬空，访问是 UB
```

### 2.3 宽/Unicode 相关字符串类型（按需求）

- `std::wstring`：`wchar_t`（Windows 通常 16-bit，Linux 常 32-bit，不统一）
- `std::u16string/std::u32string/std::u8string`：`char16_t/char32_t/char8_t`

说明：这些类型只表示“编码单元序列”，标准库不直接提供完整的 Unicode 分词、大小写折叠、规范化等能力。

---

## 3. `std::string` 常用成员 API（带典型用法）

### 3.1 容量与访问

- `size()/length()`、`empty()`、`clear()`
- `reserve()`、`capacity()`、`resize()`、`shrink_to_fit()`（不保证一定收缩）
- `data()`：C++17 起返回可写指针（非 const 的 string）；但修改需确保不越界
- `c_str()`：返回以 `\0` 结尾的 C 字符串指针（只读视角）

```cpp
std::string s = "hello";
if (!s.empty()) {
    char c0 = s.front();
    char c1 = s[1];
    char c2 = s.at(2); // 越界会抛异常
}
```

### 3.2 拼接与修改

- `operator+=` / `append()`：拼接
- `push_back()` / `pop_back()`：末尾增删
- `insert(pos, ...)`、`erase(pos, len)`、`replace(pos, len, ...)`

```cpp
std::string s = "ab";
s += 'c';
s.append("def");
// s == "abcdef"

s.insert(2, "__");  // ab__cdef
s.erase(2, 2);       // abcdef
s.replace(1, 3, "XYZ"); // aXYZef
```

### 3.3 子串与切片

- `substr(pos, len)`：返回新的 `std::string`（会分配）
- `std::string_view` 的 `substr()`：返回新的 view（不分配）

```cpp
std::string s = "abcdef";
auto sub1 = s.substr(1, 3);         // "bcd"
std::string_view sv = s;
auto sub2 = sv.substr(1, 3);        // view: "bcd"
```

### 3.4 查找

- `find` / `rfind`
- `find_first_of` / `find_last_of`
- `find_first_not_of` / `find_last_not_of`

```cpp
std::string s = "a,b,c";
size_t p = s.find(',');
if (p != std::string::npos) {
    // ...
}
```

### 3.5 比较

- `==` 等：字典序比较
- `compare()`：返回负/0/正

```cpp
std::string a = "abc", b = "abd";
bool less = a < b;          // true
int cmp = a.compare(b);     // < 0
```

### 3.6 C++20：前后缀判断

- `starts_with(prefix)`、`ends_with(suffix)`

```cpp
std::string s = "https://example.com";
if (s.starts_with("https://")) {
    // ...
}
```

---

## 4. 输入输出与分割（split）

### 4.1 `std::getline`：按行读取

```cpp
std::string line;
while (std::getline(std::cin, line)) {
    // 处理一整行
}
```

也可指定分隔符：

```cpp
std::string token;
std::getline(std::cin, token, ',');
```

### 4.2 `istringstream`：按空白切分（常用）

特性：连续空白自动跳过；适合“tokenize”。

```cpp
#include <sstream>

std::vector<std::string> split_ws(const std::string& s) {
    std::istringstream iss(s);
    std::vector<std::string> out;
    std::string tok;
    while (iss >> tok) out.push_back(tok);
    return out;
}
```

### 4.3 手写 split（按指定字符）

适合你想控制“是否保留空字段”。

```cpp
std::vector<std::string_view> split_char(std::string_view s, char delim) {
    std::vector<std::string_view> out;
    size_t i = 0;
    while (i <= s.size()) {
        size_t j = s.find(delim, i);
        if (j == std::string_view::npos) j = s.size();
        out.push_back(s.substr(i, j - i)); // 可能为空
        i = j + 1;
        if (j == s.size()) break;
    }
    return out;
}
```

### 4.4 流输入的常见模式（`operator>>` / `getline` / `std::ws`）

**`operator>>`（提取运算符）**

- 默认按“空白符”分词（空格/Tab/换行都算分隔），会跳过前导空白。
- 适合读取 token（单词、数字、路径中不含空格的字段）。

```cpp
std::string tok;
while (std::cin >> tok) {
    // 每次读一个 token
}
```

**`getline` 读整行**

- 读取到换行符为止（换行符被丢弃），可读取包含空格的整行。

```cpp
std::string line;
while (std::getline(std::cin, line)) {
    // 一整行，可能包含空格
}
```

**`>>` 和 `getline` 混用的坑**

如果先用 `>>` 读数字/单词，流里可能残留一个换行符，紧接着的 `getline` 会读到空行。
常见处理方式是在 `getline` 前吃掉残留空白：

```cpp
int n;
std::cin >> n;
std::string line;
std::getline(std::cin >> std::ws, line); // std::ws 吃掉前导空白（含换行）
```

### 4.5 流状态位与错误处理（读到 EOF/格式不对时怎么判断）

流有几个常见状态：

- `good()`：状态正常
- `eof()`：到达 EOF
- `fail()`：格式解析失败（例如读 int 遇到字母）
- `bad()`：底层 I/O 出错（较少见）

典型用法：

```cpp
int x;
if (!(std::cin >> x)) {
    // 失败：可能是 eof，也可能是格式不匹配
    if (std::cin.eof()) {
        // 输入结束
    } else {
        // 格式错误：需要 clear() 并丢弃错误输入
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
```

### 4.6 `iomanip`：常用输出格式控制（把数字“格式化成字符串”）

常用操纵符（manipulators）：

- 宽度与填充：`std::setw(n)`, `std::setfill('0')`
- 小数精度：`std::setprecision(p)`
- 浮点表示：`std::fixed`, `std::scientific`
- 进制：`std::hex/std::dec/std::oct`，以及 `std::showbase`
- 布尔：`std::boolalpha`

```cpp
#include <iomanip>

std::cout << std::setw(4) << std::setfill('0') << 12; // 0012
std::cout << std::fixed << std::setprecision(2) << 3.14159; // 3.14
```

### 4.7 `stringstream` 的清空与复用

`std::stringstream`/`istringstream`/`ostringstream` 复用时通常需要同时：

- 清理 buffer：`ss.str("")`
- 清理状态位：`ss.clear()`

```cpp
std::stringstream ss;
ss << "a b";
std::string t;
ss >> t;

ss.str("");
ss.clear();
ss << "1 2";
```

### 4.8 文件流：`ifstream/ofstream/fstream`

用于把文件内容读成字符串/按行处理。

**按行读文件**

```cpp
#include <fstream>

std::ifstream fin("input.txt");
std::string line;
while (std::getline(fin, line)) {
    // 处理每行
}
```

**整文件读入 string（常见写法）**

```cpp
#include <fstream>
#include <sstream>

std::ifstream fin("input.txt", std::ios::binary);
std::ostringstream oss;
oss << fin.rdbuf();
std::string content = oss.str();
```

### 4.9 性能小点：`sync_with_stdio(false)` 与 `tie(nullptr)`

如果大量使用 `cin/cout`（尤其是刷题/日志较多），通常会加：

```cpp
std::ios::sync_with_stdio(false);
std::cin.tie(nullptr);
```

说明：关闭与 C `stdio` 的同步，并解除 `cin` 与 `cout` 的默认绑定刷新，可显著提升 I/O 吞吐；但此后不要混用 `printf/scanf` 和 `cin/cout`。

---

## 5. trim（裁剪空白）与大小写

### 5.1 trim（基于 `string_view`，不分配）

```cpp
#include <cctype>

std::string_view trim_ws(std::string_view s) {
    auto is_ws = [](unsigned char ch) { return std::isspace(ch) != 0; };
    while (!s.empty() && is_ws(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && is_ws(static_cast<unsigned char>(s.back())))  s.remove_suffix(1);
    return s;
}
```

⚠️ `std::isspace/std::tolower/...` 的参数必须能表示为 `unsigned char` 或 `EOF`。直接传 `char` 在 `char` 为 signed 且值为负时可能 UB，所以通常写成 `static_cast<unsigned char>(ch)`。

### 5.2 大小写转换（ASCII 场景）

```cpp
#include <algorithm>
#include <cctype>

void to_lower_ascii(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
}
```

说明：这只是 ASCII/字节级处理，不等价于 Unicode 的大小写折叠。

---

## 6. 替换与删除

### 6.1 替换所有子串（常见模板）

```cpp
void replace_all(std::string& s, std::string_view from, std::string_view to) {
    if (from.empty()) return;
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
}
```

### 6.2 删除满足条件的字符

```cpp
#include <algorithm>

void remove_chars(std::string& s, char ch) {
    s.erase(std::remove(s.begin(), s.end(), ch), s.end());
}
```

---

## 7. 数字转换（string ↔ number）

### 7.1 便捷但可能抛异常：`stoi/stod/...`

- 优点：简单
- 缺点：可能抛异常；性能通常不如 `<charconv>`；对格式更“宽松”

```cpp
int x = std::stoi("123");
double y = std::stod("3.14");
```

### 7.2 推荐：`from_chars/to_chars`（更快、无异常、不分配）

```cpp
#include <charconv>

bool parse_int(std::string_view s, int& out) {
    auto first = s.data();
    auto last  = s.data() + s.size();
    auto [ptr, ec] = std::from_chars(first, last, out);
    return ec == std::errc{} && ptr == last; // ptr==last 表示全部消费
}
```

输出数字到缓冲区：

```cpp
#include <charconv>

std::string to_string_fast(int v) {
    char buf[32];
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), v);
    if (ec != std::errc{}) return {};
    return std::string(buf, ptr);
}
```

---

## 8. 格式化与拼接

### 8.1 C++20 `std::format`（若你的标准库实现支持）

```cpp
#include <format>

std::string s = std::format("id={}, score={}", 7, 99.5);
```

说明：如果你当前编译器/标准库对 `<format>` 支持不完整，可以暂时用 `ostringstream` 或第三方 fmtlib。

### 8.2 `ostringstream`：简单可控

```cpp
#include <sstream>

std::ostringstream oss;
oss << "id=" << 7 << ", score=" << 99.5;
std::string s = oss.str();
```

---

## 9. 正则（`<regex>`）

```cpp
#include <regex>

std::regex re(R"(\d+)");
bool has_num = std::regex_search("a1b", re);
```

备注：标准 `regex` 在一些实现上性能一般；高性能场景常用 RE2/PCRE2。

---

## 10. `std::string_view` 的常见用法模式

### 10.1 作为函数参数（优先推荐）

```cpp
void log(std::string_view msg);
```

好处：调用方可以传 `std::string`、字符串字面量、子串 view，避免拷贝。

### 10.2 与 `std::string` 的边界

- 需要持久保存、需要修改、需要确保以 `\0` 结尾：用 `std::string`
- 只读、短期借用、切片：用 `std::string_view`

---

## 11. 性能与坑点（实用）

- **避免无意义拷贝**：参数用 `std::string_view` / `const std::string&`；返回值按值返回通常 OK（NRVO/移动）。
- **频繁拼接**：
  - 先 `reserve()` 预留容量
  - 或用 `std::ostringstream/std::format`
- **`string_view` 生命周期**：不要返回指向局部 `std::string` 的 `string_view`。
- **`c_str()` 指针失效**：对 `std::string` 做任何可能导致重分配的操作后，之前拿到的 `data()/c_str()` 指针可能失效。
- **字符分类 UB**：`std::isspace/std::tolower` 等要用 `unsigned char` 转换。
- **`<regex>` 性能**：不是所有实现都快；大规模文本处理慎用。

---

## 12. 快速对照：我该用哪个？

- **存储/修改字符串**：`std::string`
- **只读参数/切片**：`std::string_view`
- **按空白分词**：`std::istringstream` 或 `operator>>`
- **按行读取（含空格）**：`std::getline`
- **读写文件**：`std::ifstream/std::ofstream` + `getline` 或 `rdbuf()`
- **按字符分割且不想拷贝**：`split_char(std::string_view, delim)` + 返回 view
- **裁剪空白**：`trim_ws(std::string_view)`
- **数字解析**：优先 `std::from_chars`
- **格式化**：优先 `std::format`（可用时），否则 `ostringstream`
