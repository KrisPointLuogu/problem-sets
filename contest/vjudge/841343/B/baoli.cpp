#include <bits/stdc++.h>
using namespace std;

int n;
long long best;

void dfs(vector<long long> a, long long ops)
{
    best = max(best, ops);
    int m = (int)a.size();
    if (m < 2) return;

    set<long long> vals(a.begin(), a.end());
    for (int i = 0; i + 1 < m; i++)
    {
        if (a[i] == a[i + 1]) continue;
        vector<long long> cand;
        for (long long v : vals) cand.push_back(v);
        cand.push_back(LLONG_MAX);

        for (long long v : cand)
        {
            vector<long long> b = a;
            b[i] = v;
            b.erase(b.begin() + i + 1);
            dfs(b, ops + 1);
        }
    }
}

int main()
{
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    best = 0;
    dfs(a, 0);
    cout << best << endl;
    return 0;
}
