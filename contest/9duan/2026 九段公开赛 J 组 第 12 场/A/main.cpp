#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;

long long n;
long long c[maxn];

void init()
{
    memset(c, -1, sizeof(c));
    cin >> n;
    for (long long i = 1; i <= n; i++)
    {
        cin >> c[i];
    }
}

void work()
{
    long long ans = 0;
    sort(c + 1, c + 1 + n, greater<long long>());

    for (long long i = 1;; i += 3)
    {
        // cout << i << " " << i + 1 << " " << i + 2 << endl;
        // cout << c[i] << " " << c[i + 1] << " " << c[i + 2] << endl;
        // cout << endl;
        if (c[i] == -1)
            break;
        if (c[i + 1] == -1)
        {
            ans += c[i];
            break;
        }
        if (c[i + 2] == -1)
        {
            ans += c[i] + c[i + 1];
            break;
        }
        ans += c[i] + c[i + 1];
    }

    cout << ans << endl;
}

int main()
{
    init();
    work();
    return 0;
}
