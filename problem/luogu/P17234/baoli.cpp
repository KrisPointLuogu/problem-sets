#include <bits/stdc++.h>
using namespace std;

const int MAXN = 305;
const int INF = 1000000000;

int n;
int a[MAXN];
int used[MAXN];
int calc_mex(int l, int r)
{
    for (int i = 0; i <= n + 1; i++)
        used[i] = 0;
    for (int i = l; i <= r; i++)
    {
        if (a[i] <= n + 1)
            used[a[i]] = 1;
    }

    int mex_value = 0;
    while (used[mex_value])
        mex_value++;
    return mex_value;
}

int calc_cmin(int l, int r)
{
    int cmin = INF;
    for (int i = 1; i < l; i++)
        cmin = min(cmin, a[i]);
    for (int i = r + 1; i <= n; i++)
        cmin = min(cmin, a[i]);
    return cmin;
}

void read_input()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
}

void solve()
{
    long long ans = 0;
    for (int l = 1; l <= n; l++)
    {
        for (int r = l; r <= n; r++)
        {
            if (calc_mex(l, r) == calc_cmin(l, r))
            {
                ans++;
                // cout << l << " " << r << " " << endl;
            }
        }
    }

    cout << ans << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    read_input();
    solve();

    return 0;
}