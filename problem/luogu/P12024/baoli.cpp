#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int n, q;
string s;

void work(int l, int r)
{
    l--, r--;
    bool flag = 0;
    ll max_val = 0;
    for (int i = l; i <= r - 2; i++)
    {
        for (int j = i + 1; j <= r - 1; j++)
        {
            for (int k = j + 1; k <= r; k++)
            {
                if (!(s[j] == s[k] && s[j] != s[i]))
                    continue;
                ll val = (ll)(j - i) * (k - j);
                flag = 1;
                max_val = max(max_val, val);
            }
        }
    }
    cout << (flag ? max_val : -1) << '\n';
}

signed main()
{
    cin >> n >> q;
    cin >> s;

    while (q--)
    {
        int l, r;
        cin >> l >> r;
        work(l, r);
    }

    return 0;
}
