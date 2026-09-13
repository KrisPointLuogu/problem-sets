#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main()
{
    // 孩子们 manba out
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    sort(a.begin(), a.end());

    ll ans = 0;
    for (int l = 0; l < n;)
    {
        int r = l;
        while (r < n && a[r] == a[l])
            r++;
        ll v = a[l];
        int smaller = l;
        int cntv = r - l;

        if (smaller >= k - 1)
        {
            vector<ll> x;
            for (int j = 0; j < l; j++)
                x.push_back(a[j] ^ v);
            sort(x.begin(), x.end(), greater<ll>());
            ll sum = 0;
            for (int t = 0; t < k - 1; t++)
                sum += x[t];
            ans = max(ans, sum);
        }
        else if (smaller + cntv >= k)
        {
            ll sum = 0;
            for (int j = 0; j < l; j++)
                sum += (a[j] ^ v);
            ans = max(ans, sum);
        }

        l = r;
    }

    cout << ans << endl;
    return 0;
}