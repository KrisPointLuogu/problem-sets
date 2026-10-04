#include <bits/stdc++.h>
using namespace std;

const int maxn = 5005;
const int mod = 998244353;

int n;
int a[maxn];
int ans = 0;

int tpow[maxn];
int f[maxn];

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    sort(a + 1, a + 1 + n);

    tpow[0] = 1;
    for (int i = 1; i <= n; i++)
    {
        tpow[i] = tpow[i - 1] * 2;
        tpow[i] %= mod;
    }

    f[0] = 1;

    for (int k = 1; k <= n; k++)
    {
        int x = a[k];
        int kpow = tpow[k - 1];

        int lessx = 0;
        for (int i = 0; i <= x; i++)
        {
            lessx += f[i] % mod;
            lessx %= mod;
        }

        int more = (kpow - lessx + mod) % mod;
        ans += more % mod;
        ans %= mod;

        for (int i = 5000; i >= x; i--)
        {
            f[i] += f[i - x] % mod;
            f[i] %= mod;
        }
    }

    cout << ans << endl;

    return 0;
}
