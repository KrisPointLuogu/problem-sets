#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        int n;
        long long S;
        cin >> n >> S;

        vector<long long> t(n), v(n);

        for (int i = 0; i < n; i++)
            cin >> t[i];
        for (int i = 0; i < n; i++)
            cin >> v[i];

        vector<int> p(n);
        iota(p.begin(), p.end(), 0);

        long long ans = LLONG_MAX;

        // 枚举所有取片顺序
        do
        {
            long long curTime = 0;
            long long cost = 0;

            for (int k = 0; k < n; k++)
            {
                int id = p[k];
                long long y;

                if (k == 0)
                {
                    // 第一次取片不得早于 S
                    y = max(S, t[id]);
                }
                else
                {
                    // 相邻两次取片至少间隔 1，并且不能早于该底片允许时间
                    y = max(curTime + 1, t[id]);
                }

                curTime = y;
                cost += (y - t[id]) * v[id];
            }

            ans = min(ans, cost);

        } while (next_permutation(p.begin(), p.end()));

        cout << ans << '\n';
    }

    return 0;
}