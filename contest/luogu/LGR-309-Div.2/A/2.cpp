#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
/*
第一题exgcd?
*/
ll exgcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll d = exgcd(b, a % b, x, y);
    ll tx = x;
    x = y;
    y = tx - (a / b) * y;
    return d;
}

int main()
{
    int T;
    scanf("%d", &T);
    while (T--)
    {
        ll a, b;
        scanf("%lld %lld", &a, &b);
        ll k, t;
        ll g = exgcd(b, a, k, t); // b*k + a*t = g
        k *= (b - a) / g;         // 特解
        ll L = a / g;             // k 的周期
        k = (k % L + L) % L;
        ll lcm = b / g * a;
        ll r = (a + b * k) % lcm;
        ll lo = a + b, ans = r;
        if (ans <= lo)
            ans += (lo - ans) / lcm * lcm + lcm;
        printf("%lld\n", ans);
    }
}