#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const long long maxn = 1e6 + 5;
const long long maxe = 1e6 + 5;
typedef __int128 i128;
ll gcd(ll a, ll b)
{
    return __gcd(a, b);
}
i128 gcd128(i128 a, i128 b)
{
    return b == 0 ? a : gcd128(b, a % b);
}
struct linkList
{
    typedef struct
    {
        long long u, v, w, next;
    } edge;
    edge e[maxe];
    long long h[maxn], edge_cnt = 0;
    linkList()
    {
        edge_cnt = 0;
        memset(h, -1, sizeof(h));
    }

    // 遍历点u 周围点
    template <typename U>
    void for_each(long long u, U func)
    {
        for (long long i = h[u]; i != -1; i = e[i].next)
            func(e[i].u, e[i].v, e[i].w); // u v w
    }

    void add(long long u, long long v, long long w = 0)
    {
        e[edge_cnt] = {u, v, w, h[u]};
        h[u] = edge_cnt++;
    }
    void add2(long long u, long long v, long long w = 0)
    {
        add(u, v, w);
        add(v, u, w);
    }
    // 下标访问
    edge &operator[](long long i) { return e[i]; }
    // 返回head[u]
    long long operator()(long long u) { return h[u]; }
} e;

struct frac
{
    i128 a, b; // \frac{a}{b} (a/b)

    void add(frac bf)
    {
        // self + b -> self
        if (bf.a == 0)
            return;
        if (b == 0)
        {
            i128 g = gcd128(bf.a, bf.b);
            a = bf.a / g;
            b = bf.b / g;
            return;
        }
        i128 temp = b * bf.b; // a / temp
        i128 temp2 = bf.b * a + bf.a * b;
        i128 g = gcd128(temp, temp2);
        temp /= g;
        temp2 /= g;
        b = temp;
        a = temp2;
    }

    void div(long long divnum)
    {
        b *= divnum;
    }
};

void print_i128(i128 x)
{
    if (x == 0)
    {
        putchar('0');
        return;
    }
    if (x < 0)
    {
        putchar('-');
        x = -x;
    }
    char buf[60];
    int len = 0;
    while (x > 0)
    {
        buf[len++] = '0' + x % 10;
        x /= 10;
    }
    while (len--)
        putchar(buf[len]);
}

long long n, m;
long long cnt_out;
queue<long long> q;

long long in_deg[maxn]; // 每个点的入度
long long out_deg[maxn];
// 存topsort的结果
long long ta[maxn];
long long ta_cnt;

frac dotv[maxn];

vector<long long> outnode;

void init()
{
    cin >> n >> m;
    for (long long i = 1; i <= n; i++)
    {
        long long nnode;
        cin >> nnode;
        if (nnode == 0)
        {
            cnt_out++;
            outnode.push_back(i);
        }
        for (long long j = 1; j <= nnode; j++)
        {
            long long t;
            cin >> t;
            in_deg[t]++;
            out_deg[i]++;
            e.add(i, t);
        }
    }
}

void topsort()
{
    // 把入度为0的点加入队列里
    for (long long i = 1; i <= n; ++i) // i: 1->n
    {
        if (in_deg[i] == 0)
            q.push(i);
    }

    // 队列不空
    while (!q.empty())
    {
        long long u = q.front();
        q.pop();
        // 放到结果队列里
        ta_cnt++;
        ta[ta_cnt] = u;

        for (long long i = e.h[u]; ~i; i = e.e[i].next)
        {
            long long v = e.e[i].v;
            --in_deg[v];
            if (in_deg[v] == 0)
                q.push(v);
        }
    }
}

void work()
{
    for (long long i = 1; i <= m; i++)
    {
        dotv[i].a = 1;
        dotv[i].b = 1;
    }
    for (long long i = 1; i <= ta_cnt; i++)
    {
        long long u = ta[i];
        if (out_deg[u] == 0)
            continue;
        frac temp = dotv[u];
        temp.div(out_deg[u]);
        for (long long j = e.h[u]; j != -1; j = e.e[j].next)
        {
            long long v = e.e[j].v;
            dotv[v].add(temp);
        }
    }
    for (auto p : outnode)
    {
        // p : outnode
        if (dotv[p].a == 0)
            printf("0 1\n");
        else
        {
            print_i128(dotv[p].a);
            putchar(' ');
            print_i128(dotv[p].b);
            putchar('\n');
        }
    }
}

int main(long long argc, char const *argv[])
{
    init();
    topsort();
    work();
    return 0;
}
