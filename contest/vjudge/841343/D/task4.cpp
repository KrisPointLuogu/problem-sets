#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
const int maxe = 1e6 + 5;

struct linkList
{
    typedef struct
    {
        int u, v, w, next;
    } edge;
    edge e[maxe];
    int h[maxn], edge_cnt = 0;
    linkList()
    {
        edge_cnt = 0;
        memset(h, -1, sizeof(h));
    }

    // 遍历点u 周围点
    template <typename U>
    void for_each(int u, U func)
    {
        for (int i = h[u]; i != -1; i = e[i].next)
            func(e[i].u, e[i].v, e[i].w); // u v w
    }

    void add(int u, int v, int w = 0)
    {
        e[edge_cnt] = {u, v, w, h[u]};
        h[u] = edge_cnt++;
    }
    void add2(int u, int v, int w = 0)
    {
        add(u, v, w);
        add(v, u, w);
    }
    // 下标访问
    edge &operator[](int i) { return e[i]; }
    // 返回head[u]
    int operator()(int u) { return h[u]; }
} e;

const int maxnfloydn = 100;
const int inf = 0x7f7f7f7f / 3;
const int mod = 1e9 + 7;

int f[maxnfloydn][maxnfloydn];

int n, m;

void floyd()
{
    int k, i, j;
    for (k = 1; k <= n; k++)
    { // k在最外层
        for (i = 1; i <= n; i++)
        {
            for (j = 1; j <= n; j++)
            {
                if (f[i][k] + f[k][j] < f[i][j])
                { // 松弛法,如果能更小,那就更小
                    f[i][j] = f[i][k] + f[k][j];
                }
            }
        }
    }
}

long long func(int u, int v, int i)
{
    int minv = INT32_MAX;
    for (int x = 1; x <= n; x++)
    {
        const int left = f[u][x] + f[v][x];
        const int right = f[u][v];
        if (left == right)
        {
            const int val = f[x][i];
            minv = min(minv, val);
        }
    }

    return minv;
}

void work_task4_on2()
{
    long long ans = 0;
    for (int u = 1; u <= n; u++)
    {
        for (int v = u + 1; v <= n; v++)
        {
            // printf("u : %d,v : %d\n", u, v);
            const long long left = 1LL * u * (u - 1) / 2 % mod;
            const long long right = 1LL * (n - v) * (n - v + 1) / 2 % mod;
            // printf("left : %d,right : %d\n", left, right);
            ans = (ans + left + right) % mod;
        }
    }
    cout << ans << endl;
}

void work_task4_on()
{
    long long ans = 0;
    for (int u = 1; u <= n; u++)
    {
        const long long left = 1LL * (n - u) * u * (u - 1) / 2 % mod;
        const long long r = n - u - 1;
        const long long right = r * (r + 1) * (r + 2) / 6 % mod;
        ans = (ans + left + right) % mod;
    }
    cout << ans << endl;
}

int main(int argc, char const *argv[])
{
    cin >> n;
    m = n - 1;
    bool task4 = true;

    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        if (!(u == i && v == i + 1))
            task4 = false;
        e.add2(u, v);
        if (u < maxnfloydn && v < maxnfloydn)
            f[u][v] = f[v][u] = 1;
    }
    if (task4)
    {
        work_task4_on();
        return 0;
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (f[i][j])
                continue;
            f[i][j] = inf;
            if (i == j)
                f[i][j] = 0;
        }
    }
    floyd();
    long long ans = 0;
    for (int u = 1; u <= n; u++)
    {
        for (int v = u + 1; v <= n; v++)
        {
            for (int i = 1; i <= n; i++)
            {
                ans += func(u, v, i) % mod;
            }
        }
    }

    cout << ans << endl;

    return 0;
}
