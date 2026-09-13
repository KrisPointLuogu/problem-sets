#include <bits/stdc++.h>
using namespace std;

int T;
const int maxn = 1e6 + 5;
int a[maxn];
int n, A;
int b[maxn];

int dfs(int dep, int m, int cnt);
void work()
{
    cin >> n >> A;
    if (n == 1)
    {
        cout << -1 << endl;
        return;
    }
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    sort(a + 1, a + 1 + n, greater<int>());
    for (int i = 1; i <= n; i++)
    {
        if (dfs(1, i, 0))
        {
            cout << i << endl;
            return;
        }
    }
    cout << -1 << endl;
}

int dfs(int dep, int m, int cnt)
{
    if (cnt > m)
        return 0;

    if (dep > n && cnt < m)
        return 0;

    if (dep > n && cnt == m)
    {
        int sum = 0;
        sum = A;
        for (int i = 1; i <= n; i++)
        {
            if (b[i])
            {
                sum %= a[i];
            }
        }

        if (sum == 0)
        {
            return 1;
        }

        return 0;
    }
    for (int i = 0; i <= 1; i++)
    {
        b[dep] = i;
        if (dfs(dep + 1, m, cnt + i))
            return 1;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    cin >> T;
    while (T--)
    {
        work();
    }
    return 0;
}
