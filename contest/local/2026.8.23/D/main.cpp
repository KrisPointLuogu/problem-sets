#include <bits/stdc++.h>
using namespace std;

vector<double> v;
priority_queue<double> pq;

vector<pair<int, int>> vp;

double f(int x, int y)
{
    double a = vp[x - 1].first * vp[x - 1].second;
    double b = vp[y - 1].first * vp[y - 1].second;
    double c = a * 1.0 + b * 1.0;
    double d = (vp[x - 1].first + vp[y - 1].first) * 1.0;
    double ret = c * 1.0 / d * 1.0;
    return ret;
}

int main(int argc, char const *argv[])
{
    int n, k;
    cin >> n >> k;
    bool flag = 0;
    if (k == 1)
        flag = 1;
    for (int i = 1; i <= n; i++)
    {
        int p, q;
        cin >> p >> q;
        vp.push_back(make_pair(p, q));
    }

    double maxans = INT32_MIN;

    for (int j = 1; j <= n; j++)
    {
        for (int i = 1; i < j; i++)
        {
            double num = f(i, j);
            maxans = max(maxans, num);
            // cout << i << " " << j << " ";
            // cout << fixed << setprecision(3) << num << endl;
            v.push_back(num);
        }
    }
    if (!flag)
    {
        sort(v.begin(), v.end(), greater<double>());
        cout << fixed << setprecision(3) << v[k - 1] << endl;
    }
    else
    {
        cout << fixed << setprecision(3) << maxans << endl;
    }
    return 0;
}
