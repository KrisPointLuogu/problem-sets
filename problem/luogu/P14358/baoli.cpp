#include <bits/stdc++.h>
using namespace std;

int n, m;
const int maxn = 1e3 + 5;

int a[maxn];

void dfs(int i, int j, int cnt)
{
    if (a[1] > a[cnt])
    {
        cout << j << " " << i << endl;
        exit(0);
    }
    if (j & 1)
    {
        if (i < n)
            dfs(i + 1, j, cnt + 1);
        else
            dfs(i, j + 1, cnt + 1);
    }
    else
    {
        if (i > 1)
            dfs(i - 1, j, cnt + 1);
        else
            dfs(i, j + 1, cnt + 1);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n >> m;
    for (int i = 1; i <= n * m; i++)
    {
        cin >> a[i];
    }
    sort(a + 2, a + 1 + n * m, greater<int>());

    dfs(1, 1, 2);

    return 0;
}
