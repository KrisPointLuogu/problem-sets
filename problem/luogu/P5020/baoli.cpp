#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
const int maxa = 25000;

int n;
int a[maxn];
bool vis[maxa + 5];

void reinit()
{
    n = 0;
    memset(a, 0, sizeof a);
    memset(vis, 0, sizeof vis);
}

void init()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    sort(a + 1, a + 1 + n);
}

void work()
{
    vis[0] = true;
    int ans = 0;
    for (int i = 1; i <= n; i++)
    {
        if (vis[a[i]])
        {
            continue;
        }
        ans++;
        for (int j = a[i]; j <= maxa; j++)
        {
            if (vis[j - a[i]])
            {
                vis[j] = true;
            }
        }
    }

    cout << ans << endl;
}

int main(int argc, char const *argv[])
{
    int t;
    cin >> t;
    // init();
    while (t--)
    {
        reinit();
        init();
        work();
    }
    return 0;
}
