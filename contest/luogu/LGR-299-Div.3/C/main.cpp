#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;

long long n, x, q;

struct machine
{
    long long o;
    long long a, b;
    long long c;
} m[maxn];

bool task4 = 1;

void init()
{
    cin >> n >> x >> q;
    for (long long i = 1; i <= n; i++)
    {
        cin >> m[i].o >> m[i].a >> m[i].b;
        if (m[i].a != m[i].b)
            task4 = 0;
        if (m[i].o == 1)
        {
            cin >> m[i].c;
        }
    }
}

set<long long> ans;

void dfs(long long pos, long long sum)
{
    if (pos == n + 1)
    {
        ans.insert(sum);
        return;
    }
    if (m[pos].o == 1)
    {
        if (sum >= m[pos].a)
        {
            dfs(pos + 1, sum - m[pos].a + m[pos].b);
            dfs(pos + 1, sum - m[pos].a + m[pos].c);
        }
        dfs(pos + 1, sum);
    }
    if (m[pos].o == 0)
    {
        if (sum >= m[pos].a)
            dfs(pos + 1, sum - m[pos].a + m[pos].b);
        else
            dfs(pos + 1, sum);
    }
}

void work(long long need_to_sum)
{
    cout << ans.count(need_to_sum) << endl;
}

const int maxn4 = 6000005;
static bitset<maxn4> B;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    init();
    if (task4)
    {
        B.reset();
        B[x] = 1;
        for (long long i = 1; i <= n; i++)
        {
            if (m[i].o == 1)
            {
                B |= (B >> m[i].a) << m[i].c;
            }
        }
        while (q--)
        {
            long long y;
            cin >> y;
            cout << (y < maxn4 && B[y]) << endl;
        }
        return 0;
    }
    dfs(1, x);
    while (q--)
    {
        long long y;
        cin >> y;
        work(y);
    }
    return 0;
}
