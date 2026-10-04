/**
 * oj: luogu
 * title: [CSP-J 2023] 旅游巴士
 * description: 10 分部分分：k = 1 且 a_i = 0，最普通的 BFS 求最短路
 */
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

template <typename T>
inline void read(T &x)
{
    x = 0;
    T f = 1;
    char c = getchar();
    while (c < '0' || c > '9')
    {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9')
    {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x = x * f;
}

template <typename T, typename... Args>
inline void read(T &x, Args &...args)
{
    read(x);
    read(args...);
}

template <typename T>
inline void write(T x)
{
    if (x < 0)
    {
        putchar('-');
        x = -x;
    }
    if (x == 0)
    {
        putchar('0');
        return;
    }
    char buf[50];
    int len = 0;
    while (x > 0)
    {
        buf[len++] = x % 10 + '0';
        x /= 10;
    }
    for (int i = len - 1; i >= 0; i--)
        putchar(buf[i]);
}

const int maxn = 1e4 + 5;

int n, m, k;
vector<int> adj[maxn];
int dis[maxn];

int main()
{
#ifdef FREOPEN
    freopen("in", "r", stdin);
#endif
    read(n, m, k);

    // k = 1：任意时刻都是 1 的整数倍，a_i = 0 时无须等待
    // 问题退化为无权有向图 1 到 n 的最短路，直接 BFS
    for (int i = 0; i < m; i++)
    {
        int u, v, a;
        read(u, v, a);
        adj[u].push_back(v);
    }

    for (int i = 1; i <= n; i++)
        dis[i] = -1;

    queue<int> q;
    dis[1] = 0;
    q.push(1);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : adj[u])
        {
            if (dis[v] == -1)
            {
                dis[v] = dis[u] + 1;
                q.push(v);
            }
        }
    }

    write(dis[n]);
    putchar('\n');

    return 0;
}
