#include <bits/stdc++.h>
using namespace std;

const int maxn = 200005;

int n;
int cnt[maxn];

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        int x;
        cin >> x;
        cnt[x]++;
    }

    int missing_lt_i = 0;

    for (int i = 0; i <= n; i++)
    {
        cout << max(cnt[i], missing_lt_i) << '\n';

        if (cnt[i] == 0)
        {
            missing_lt_i++;
        }
    }

    return 0;
}