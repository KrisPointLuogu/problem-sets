/**
 * oj: luogu
 * title: [CSP-J 2023] 旅游巴士
 * description: 特殊性质 a_i = 0，BFS 求最早离开时刻
 */
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// 简化版快速IO模板
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
const int maxk = 105;

int n, m, k;
vector<int> adj[maxn];
int dist[maxn][maxk];

void init()
{
    read(n, m, k);
    for (int i = 0; i < m; i++)
    {
        int u, v, a;
        read(u, v, a);
        adj[u].push_back(v);
    }
}

void solve()
{
    // 所有 a_i = 0，边权均为 1，直接 BFS
    // 状态 (u, r)：当前在 u，已经用时 t，r = t % k
    // dist[u][r] 表示到达该状态的最早时间
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < k; j++)
            dist[i][j] = -1;

    queue<pair<int, int>> q;
    dist[1][0] = 0; // 0 时刻到达入口，是 k 的整数倍
    q.push({1, 0});

    while (!q.empty())
    {
        int u = q.front().first;
        int r = q.front().second;
        q.pop();

        for (int v : adj[u])
        {
            int nr = (r + 1) % k;
            if (dist[v][nr] == -1)
            { // 首次到达即为最早时间
                dist[v][nr] = dist[u][r] + 1;
                q.push({v, nr});
            }
        }
    }

    // 离开时间必须是 k 的非负整数倍，即余数为 0
    write(dist[n][0]);
    putchar('\n');
}

signed main()
{
#ifdef FREOPEN
    freopen("in", "r", stdin);
#endif
    init();
    solve();
    return 0;
}
