#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int T;

int n;
int a[maxn];

void init()
{
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
}

int b[maxn];

int next_pos(int pos)
{
    return (pos + 1) % n;
}

int prev_pos(int pos)
{
    return (pos - 1) % n;
}

int tmp[maxn];

void do_it(int pos)
{
    int tp = tmp[pos];
    tmp[pos] = tmp[next_pos(pos)] + tmp[prev_pos(pos)];
    tmp[pos] -= tp;
}
long long minans = INT32_MAX;
void dfs(int dep)
{
    if (dep > n)
    {
        memcpy(tmp, a, sizeof a);
        // b[i] -> 操作数i
        for (int i = 1; i <= n; i++)
        {
            if (i != n)
                do_it(i);
        }
        int cnt = 0;
        for (int i = 0; i < n; i++)
        {
            if (a[prev_pos(i)] < a[i] && a[next_pos(i)] < a[i])
            {
                cnt++;
            }
            if (cnt >= 2)
                return;
        }
        long long ans = 0;
        for (int i = 1; i <= n; i++)
        {
            if (a[i] != n)
                ans++;
        }

        minans = min(minans, ans);

        return;
    }
    for (int i = 0; i <= n; i++)
    {
        b[dep] = i;
        dfs(dep + 1);
    }
}

void work()
{
    minans = INT32_MAX;
    dfs(1);
    cout << minans << endl;
}

int main(int argc, char const *argv[])
{
    cin >> T;
    while (T--)
    {
        init();
        work();
    }
    return 0;
}
