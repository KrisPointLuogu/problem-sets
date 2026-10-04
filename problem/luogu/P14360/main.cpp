#include <bits/stdc++.h>
using namespace std;

const int maxn = 5005;
const int mod = 998244353;

int n;
int a[maxn];
int b[maxn];

int ans = 0;

void dfs(int dep, int cnt)
{
    if (dep > n)
    {
        // m -> cnt
        if (cnt < 3)
            return;
        long long sum = 0;
        long long maxans = -1;
        for (int i = 1; i <= n; i++)
        {
            if (b[i])
            {
                sum += a[i];
                maxans = max(maxans, (long long)a[i]);
            }
        }
        int compare_right = 2 * maxans;

        if (sum > compare_right)
        {
            ans++;
            ans %= mod;
        }

        return;
    }

    for (int i = 0; i <= 1; i++)
    {
        b[dep] = i;
        dfs(dep + 1, cnt + i);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    dfs(1, 0);

    cout << ans << endl;

    return 0;
}
