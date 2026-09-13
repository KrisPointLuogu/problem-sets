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
        reset();
    }

    void reset()
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
    int operator()(int i) { return h[i]; }
} e;

int sz[maxn];        // sz[u]：以 u 为根的子树大小
long long dep_sum;   // 以 1 为根的所有点深度之和
long long ans[maxn]; // ans[u]：以 u 为会议地点的距离和
int n, m;

void dfs(int u, int fa, long long dep)
{
    sz[u] = 1;
    dep_sum += dep;
    e.for_each(u, [&](int x, int v, int w)
               {
        if (v == fa)
            return;
        dfs(v, u, dep + 1);
        sz[u] += sz[v]; });
}

void dfs2(int u, int fa)
{
    if (fa != 0)
        ans[u] = ans[fa] + n - 2LL * sz[u];
    e.for_each(u, [&](int x, int v, int w)
               {
        if (v == fa)
            return;
        dfs2(v, u); });
}

int main(int argc, char const *argv[])
{
    cin >> n;
    m = n - 1;
    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        e.add2(u, v, 1);
    }

    dfs(1, 0, 0);
    ans[1] = dep_sum;
    dfs2(1, 0);

    long long minans = ans[1];
    int best = 1;
    for (int i = 2; i <= n; i++)
    {
        if (ans[i] < minans)
        {
            minans = ans[i];
            best = i;
        }
    }
    cout << best << " " << minans << endl;

    return 0;
}
