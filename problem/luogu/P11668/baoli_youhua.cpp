#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
int n;
int a[maxn];
int second_to_last[maxn];
int cnt_right[maxn];
bool seen[maxn];

#define int long long

signed main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i];

    for (int i = n; i >= 1; i--)
    {
        cnt_right[a[i]]++;
        if (cnt_right[a[i]] == 2)
            second_to_last[a[i]] = i;
    }

    int ans = 0;
    int prefix_distinct = 0;
    for (int i = 1; i <= n; i++)
    {
        if (!seen[a[i]])
        {
            seen[a[i]] = true;
            prefix_distinct++;
        }
        if (second_to_last[a[i]] == i)
            ans += prefix_distinct - 1;
    }

    cout << ans << '\n';

    return 0;
}
