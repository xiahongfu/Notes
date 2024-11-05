[最浅显易懂的 KMP 算法讲解](https://www.bilibili.com/video/BV1AY4y157yL/?spm_id_from=333.337.search-card.all.click&vd_source=fec9159eb85cccb827de45e19eb060e7)
[KMP的next数组求法详解](https://blog.csdn.net/to_be_better/article/details/49086075)
KMP 算法能够在O(M+N)时间复杂度内实现字符串的模式匹配问题。其主要思想是使用模式串的最长公共前后缀数组来去除重复判断操作。

下面用一个例子介绍如何计算next数组。
```txt
index =  0  1  2  3  4  5  6
chars =  a  b  a  b  a  a  c
next =  [0, 0, 1, 2, 3, 1, 0]

0 1 2 3 4 5 6
a b a x x x x
x x a b a x x

next数组是最长公共前后缀（可能有别的定义，next定义成最长公共前后缀更好理解）

在上面这个例子中求next[5]时。假设已经知道了next[0]~next[4]。
next[4]=3，所以ababa的最长公共前后缀长度为3。也就是前缀aba（下标0~2）和后缀aba（下标2~4）相同。
因此如果s[3] == s[5]，那么最长公共前后缀长度增加一即可，变成4。

如果s[3] != s[5]，由于aba的最长公共前后缀为1，因此下标0~2的前缀和下标2~4的后缀最长也为1（也就是a）。
此时只要判断s[1] ?= s[5]即可。如果相等，那么最长公共前后缀变成2，如果不等，重复上面过程判断s[0] ?= s[5]即可。
```

下面这个例子是kmp算法利用next数组进行求解
```txt
s = abcabcabd
p = abcabd
next = [0, 0, 0, 1, 2, 0]

当对比到s[5]和p[5]时发现不等。
根据next表可知abcab有公共前后缀ab。
因此s的后缀ab与后面一个字符组成的子串，可能和p的后缀ab与后面一个字符组成的子串相等。
因此对比s[5]和p[2]即可。

kmp算法对比字符串时，由于指向s的指针不回头，因此时间复杂度只有O(m+n)
```

[找出字符串中第一个匹配项的下标](https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/)
```java
class Solution {
    public int strStr(String haystack, String needle) {
        int[] next = getNext(needle.toCharArray());
        char[] s = haystack.toCharArray();
        char[] p = needle.toCharArray();

        // k可以理解成模式串的待匹配下标。也可以理解成匹配了的长度
        for (int i = 0, k = 0; i < s.length; i++) {
            while (k > 0 && s[i] != p[k]) { // 如果当前模式串下标k不匹配，就将下标k移动到下一个可能匹配的地方
                k = next[k-1];
            }

            if (s[i] == p[k]) {  // 当前模式串下标k匹配，将下标k和下标i都往后移。
                k++;

                if (k == next.length) {
                    return i - k + 1;
                }
            }
            // 如果不匹配，k一定为0。只需要将下标i往后移即可。
        }
        return -1;
    }

    public int[] getNext(char[] s) {
        //  a  b  a  b  a  a  c
        // [0, 0, 1, 2, 3, 1, 0]
        int[] next = new int[s.length];
        next[0] = 0;
        // k可以理解成 待匹配的前缀下标。也可以理解成匹配成功的前后缀长度。
        for (int i = 1, k = 0; i < s.length; i++) {
            while (k > 0 && s[k] != s[i]) { // 如果当前前缀下标k不匹配，就将下标k移动到下一个可能匹配的地方
                k = next[k-1];
            }
            if (s[k] == s[i]) {  // 当前前缀下标k匹配，将下标k和下标i都往后移。让下次循环判断下一个下标是否匹配。
                k++;
            }
            next[i] = k; // k是下标也是长度
            // 如果不匹配，那么k一定为0。只需要将i往后移即可。
        }
        return next;
    }
}
```