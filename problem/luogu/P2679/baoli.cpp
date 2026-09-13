#include <bits/stdc++.h>
using namespace std;

int n, m, k;
string a, b;
long long ans;

void dfs(int a_pos, int b_pos, int seg)
{
    if (seg == k)
    {
        if (b_pos == m)
            ans = (ans + 1) % 1000000007;
        return;
    }
    if (a_pos >= n || b_pos >= m)
    {
        return;
    }
    dfs(a_pos + 1, b_pos, seg);
    for (int len = 1; a_pos + len - 1 < n && b_pos + len - 1 < m; len++)
    {
        if (a[a_pos + len - 1] != b[b_pos + len - 1])
        {
            break;
        }
        dfs(a_pos + len, b_pos + len, seg + 1);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n >> m >> k;
    cin >> a >> b;

    ans = 0;
    dfs(0, 0, 0);
    cout << ans << endl;

    return 0;
}
