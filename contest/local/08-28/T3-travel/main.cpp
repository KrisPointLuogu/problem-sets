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

    void reset()
    {
        edge_cnt = 0;
        memset(h, -1, sizeof(h));
        memset(e, 0, sizeof(0));
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
    void del(int u, int v)
    {
        e[edge_cnt] = {0, 0, 0, 0};
        h[u] = edge_cnt--;
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
int n, m;

int color[maxn], color_cnt = 0, DFN = 0; // 每个点的颜色,也就是属于的连通分量的编号
bool instack[maxn];
stack<int> sta; // 栈

int dfn[maxn], low[maxn];
int vis[maxn];

void tarjan(int u)
{
    vis[u] = 1;
    dfn[u] = low[u] = ++DFN; // 为节点u设定次序编号和low初值
    sta.push(u);             // 将节点u压入栈中
    for (int i = e(u); ~i; i = e[i].next)
    {
        int v = e[i].v;
        if (!dfn[v])
        {              // 如果节点v未被访问过
            tarjan(v); // 继续向下找
            low[u] = min(low[u], low[v]);
        }
        else if (instack[v])
        {                                 // 反祖边,节点v还在栈内
            low[u] = min(low[u], dfn[v]); // low[u] = min(low[u], low[v]) 理论上这样写也可以
        }
    }
    if (dfn[u] == low[u])
    { // 如果节点u是强连通分量的根
        color_cnt++;
        int t = -1;
        do
        {
            t = sta.top();
            sta.pop();
            instack[t] = 0; // 将v退栈，为该强连通分量中一个顶点
            color[t] = color_cnt;
        } while (t != u);
    }
}

vector<pair<int, int>> vp;

void reset()
{
    memset(vis, 0, sizeof vis);
    e.reset();
    for (int i = 1; i <= m; i++)
    {
        e.add(vp[i - 1].first, vp[i - 1].second);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n >> m;
    for (int i = 1; i <= m; i++)
    {
        int p, q;
        cin >> p >> q;
        vp.push_back({p, q});
        e.add(vp[i - 1].first, vp[i - 1].second);
    }

    for (int j = 1; j <= n; j++)
    {                    // 遍历所有点
        if (vis[j] == 0) // 没有被访问过
            tarjan(j);
    }
    reset();

    for (int i = 1; i <= m; i++)
    {
        e.add(vp[i - 1].second, vp[i - 1].first);
        // work
        for (int j = 1; j <= n; j++)
        {                    // 遍历所有点
            if (vis[j] == 0) // 没有被访问过
                tarjan(j);
        }

        // reset
        reset();
        return 0;
    }
