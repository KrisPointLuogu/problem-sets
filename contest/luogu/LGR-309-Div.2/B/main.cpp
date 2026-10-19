#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll pa[5005], pb[5005];
int pre[5005], cur[5005];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T;
    cin >> T;
    while (T--)
    {
        int n, m;
        cin >> n >> m;
        ll s = 0;
        pa[0] = 0;
        for (int i = 1; i <= n; i++)
        {
            ll x;
            cin >> x;
            s += x;
            pa[i] = s;
        }
        ll sa = s;
        s = 0;
        pb[0] = 0;
        for (int i = 1; i <= m; i++)
        {
            ll x;
            cin >> x;
            s += x;
            pb[i] = s;
        }
        if (sa != s)
        {
            cout << -1 << '\n';
            continue;
        }

        for (int j = 0; j <= m; j++)
            pre[j] = 0;
        for (int i = 1; i <= n; i++)
        {
            cur[0] = 0;
            for (int j = 1; j <= m; j++)
            {
                if (pa[i] == pb[j])
                    cur[j] = pre[j - 1] + 1;
                else
                    cur[j] = max(pre[j], cur[j - 1]);
            }
            for (int j = 0; j <= m; j++)
                pre[j] = cur[j];
        }
        cout << (n + 1) + (m + 1) - 2 * pre[m] << '\n';
    }
    return 0;
}