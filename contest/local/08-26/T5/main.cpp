#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
const int mod = 998244353;

int n;
int a[maxn];
int b[maxn];

int ans = 0;

bool check(int b[])
{
    for (int i = 2; i <= n; i++)
    {
        if (b[i] == b[i - 1])
            return 0;
    }
    return 1;
}

void dfs(int dep)
{
    if (dep > n)
    {
        // for (int i = 1; i <= n; i++)
        // {
        //     cout << b[i] << ' ';
        // }
        // cout << endl;

        if (check(b))
        {
            ans++;
            ans %= mod;
        }
        return;
    }
    for (int i = 1; i <= a[dep]; i++)
    {
        b[dep] = i;
        dfs(dep + 1);
    }
}

void init()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
}

int main(int argc, char const *argv[])
{
    init();
    dfs(1);
    cout << ans << endl;
    return 0;
}
