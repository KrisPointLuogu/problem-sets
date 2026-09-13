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

void init()
{
    cin >> n >> x >> q;
    for (long long i = 1; i <= n; i++)
    {
        cin >> m[i].o >> m[i].a >> m[i].b;
        if (m[i].o == 1)
        {
            cin >> m[i].c;
        }
    }
}

set<long long> ans;

void dfs(long long pos, long long sum)
{
    // cout << "[DFS STATE] " << pos << " " << sum << " " << need_sum << endl;
    // cout << "[m[pos] STATE] " << m[pos].o << " " << m[pos].a << " " << m[pos].b << " " << m[pos].c;
    // cout << endl;

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

int main()
{
    init();
    dfs(1, x);
    while (q--)
    {
        long long y;
        cin >> y;
        work(y);
    }
    return 0;
}
