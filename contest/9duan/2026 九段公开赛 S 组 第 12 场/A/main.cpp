#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;

long long T;

long long n, S;
struct photo
{
    long long t, v;
} p[maxn];

bool flag_A = 1;
bool flag_B = 1;

void init()
{
    flag_A = flag_B = true;
    cin >> n >> S;
    for (long long i = 1; i <= n; i++)
    {
        cin >> p[i].t;
        if (i != 1)
        {
            if (p[i].t != p[i - 1].t)
                flag_A = 0;
        }
    }
    for (long long i = 1; i <= n; i++)
    {
        cin >> p[i].v;
        if (i != 1)
        {
            if (p[i].v != p[i - 1].v)
                flag_B = 0;
        }
    }
}

void work()
{
    vector<long long> ord(n);
    iota(ord.begin(), ord.end(), 1);

    sort(ord.begin(), ord.end(), [&](long long x, long long y)
         { return max(p[x].t, S) < max(p[y].t, S); });

    priority_queue<pair<long long, long long>> pq; // (v, id)

    long long cur = 0;
    long long ans = 0;
    long long i = 0;

    while (i < n || !pq.empty())
    {
        if (pq.empty())
        {
            long long id = ord[i];
            cur = max(cur, (long long)max(p[id].t, S));
        }

        while (i < n && (long long)max(p[ord[i]].t, S) <= cur)
        {
            long long id = ord[i];
            pq.push({p[id].v, id});
            i++;
        }

        auto [vv, id] = pq.top();
        pq.pop();

        ans += (cur - p[id].t) * 1LL * p[id].v;
        cur++;
    }

    cout << ans << '\n';
}
bool cmp_A(const photo &a, const photo &b)
{
    return a.v > b.v;
}

void work_A()
{
    // 所有 t 相等
    sort(p + 1, p + 1 + n, cmp_A);
    long long time = max((long long)S, (long long)p[1].t);
    long long ans = 0;
    for (long long i = 1; i <= n; i++, time++)
    {
        ans += (time - p[i].t) * 1LL * p[i].v;
    }
    cout << ans << '\n';
}

bool cmp_B(const photo &a, const photo &b)
{
    return a.t < b.t;
}

void work_B()
{
    // 所有 v 相等
    sort(p + 1, p + 1 + n, cmp_B);
    long long time = max((long long)S, (long long)p[1].t);
    long long ans = 0;
    for (long long i = 1; i <= n; i++, time++)
    {
        time = max(time, (long long)p[i].t);
        ans += (time - p[i].t) * 1LL * p[i].v;
    }
    cout << ans << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--)
    {
        init();
        if (flag_A)
        {
            work_A();
        }
        else if (flag_B)
        {
            work_B();
        }
        else
        {
            work();
        }
    }
    return 0;
}