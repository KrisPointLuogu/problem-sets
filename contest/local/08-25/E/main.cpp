#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int n;
int a[maxn];
int b[maxn];

int ans = 0;

void dfs(int dep)
{
    if (dep > n)
    {
        int ora = 0, orb = 0;
        for (int i = 1; i <= n; i++)
        {
            if (b[i])
            {
                ora |= a[i];
            }
            else
            {
                orb |= a[i];
            }
        }
        if (ora == orb)
            ans++;

        return;
    }
    for (int i = 0; i <= 1; i++)
    {
        b[dep] = i;
        dfs(dep + 1);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    dfs(1);

    cout << ans << endl;

    return 0;
}
