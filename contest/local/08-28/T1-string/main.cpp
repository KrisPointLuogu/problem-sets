#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
int preB[maxn];
int sufA[maxn];

int main()
{
    string s;
    cin >> s;
    int n = s.size();

    for (int i = 1; i <= n; i++)
    {
        preB[i] = preB[i - 1] + (s[i - 1] == 'B');
    }

    for (int i = n; i >= 1; i--)
    {
        sufA[i] = sufA[i + 1] + (s[i - 1] == 'A');
    }

    int ans = n;
    for (int k = 0; k <= n; k++)
    {
        int cost = preB[k] + sufA[k + 1];
        ans = min(ans, cost);
    }

    cout << ans << endl;
    return 0;
}