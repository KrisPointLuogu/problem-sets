#include <bits/stdc++.h>
using namespace std;

int k, n;
vector<vector<int>> a;
vector<int> lis;
int ans;

int f[1000005];
int b[1000005];

void dfs(int i)
{
    if (i == n)
    {
        ans = max(ans, (int)lis.size());
        return;
    }
    for (int j = 0; j < k; j++)
    {
        int val = a[i][j];
        int p = -1;
        for (int t = 0; t < (int)lis.size(); t++)
        {
            if (lis[t] >= val)
            {
                p = t;
                break;
            }
        }
        if (p == -1)
        {
            lis.push_back(val);
            dfs(i + 1);
            lis.pop_back();
        }
        else
        {
            int old = lis[p];
            lis[p] = val;
            dfs(i + 1);
            lis[p] = old;
        }
    }
}

int main()
{
    cin >> k >> n;
    for (int i = 0; i < n; i++)
    {
        vector<int> t;
        for (int j = 0; j < k; j++)
        {
            int tmp = 0;
            cin >> tmp;
            t.push_back(tmp);
        }
        a.push_back(t);
    }
    if (n == 1)
    {
        ans = 1;
        goto labelend;
    }
    dfs(0);
labelend:
    cout << ans << "\n";
    return 0;
}
