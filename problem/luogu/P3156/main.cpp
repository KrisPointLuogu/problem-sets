#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main()
{

    ll n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (ll i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    for (ll i = 1; i <= m; i++)
    {
        ll x;
        cin >> x;
        cout << a[x] << '\n';
    }

    return 0;
}