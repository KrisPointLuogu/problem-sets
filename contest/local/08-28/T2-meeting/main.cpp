#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

struct people
{
    int x, v;
    int l, r;
} p[maxn];

int n;
double ans; // time

bool check(double T)
{
    double L = -1e18, R = 1e18;
    for (int i = 1; i <= n; i++)
    {
        L = max(L, p[i].x - p[i].v * T);
        R = min(R, p[i].x + p[i].v * T);
        if (L > R)
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    cin >> n;

    int maxnum = -1;
    int minnum = INT32_MAX;

    for (int i = 1; i <= n; i++)
    {
        cin >> p[i].x;
        maxnum = max(maxnum, p[i].x);
        minnum = min(minnum, p[i].x);
    }

    for (int i = 1; i <= n; i++)
    {
        cin >> p[i].v;
    }

    double l = 0, r = 1e9;
    for (int it = 0; it < 200; it++)
    {
        double mid = (l + r) / 2;
        if (check(mid))
            r = mid;
        else
            l = mid;
    }
    cout << fixed << setprecision(5) << l << endl;
    return 0;
}
