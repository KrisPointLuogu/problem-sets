/*
给你一个序列 $a_1, a_2, \ldots, a_n$。
你可以对这个序列进行若干次操作。
设一次操作前序列长度为 $m$，
那么这次操作你可以选择一个整数 $i$
使得 $1 \le i \le m - 1$ 且 $a_i \ne a_{i + 1}$，
删除 $a_{i + 1}$ 并把 $a_i$ 的值设成**任意整数**。
求你最多能进行多少次操作。

## 输入格式
第一行包含一个正整数 $n$，表示序列的初始长度。
第二行包含 $n$ 个正整数 $a_1, a_2, \ldots, a_n$。
*/

#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

long long n;
long long a[maxn];

set<long long> sa;

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        sa.insert(a[i]);
    }

    if (sa.size() == 1)
    {
        cout << 0 << endl;
        return 0;
    }

    cout << n - 1 << endl;

    return 0;
}
