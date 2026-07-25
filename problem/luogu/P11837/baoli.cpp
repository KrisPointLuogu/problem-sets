/**
 * Author by Rainboy blog: https://rainboylv.com github: https://github.com/rainboylvx
 * rbook: -> https://rbook.roj.ac.cn  https://rbook2.roj.ac.cn
 * rainboy的学习导航网站: https://idx.roj.ac.cn
 * create_at: 2026-07-11 15:09
 * update_at: 2026-07-11 15:11
 */
// brute.cpp：小数据暴力解，用来帮助理解题意并辅助对拍。
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 15;
const int INF = 1e9;

int n;
int a[MAXN];
int b[MAXN];
int best[MAXN];

int get_mex()
{
    bool mexb[MAXN];
    for (int i = 0; i <= n; i++)
    {
        mexb[i] = false;
    }

    for (int i = 1; i <= n; i++)
    {
        if (0 <= b[i] && b[i] <= n)
        {
            mexb[b[i]] = true;
        }
    }

    for (int i = 0; i <= n; i++)
    {
        if (!mexb[i])
            return i;
    }
    return n + 1;
}

int count_changes()
{
    int cnt = 0;
    for (int i = 1; i <= n; i++)
    {
        if (a[i] != b[i])
            cnt++;
    }
    return cnt;
}

void dfs(int dep)
{
    if (dep > n)
    {
        int mex = get_mex();
        int changes = count_changes();
        if (0 <= mex && mex <= n && best[mex] > changes)
        {
            best[mex] = changes;
        }
        return;
    }

    for (int value = 0; value <= n; value++)
    {
        b[dep] = value;
        dfs(dep + 1);
    }
}

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    for (int i = 0; i <= n; i++)
    {
        best[i] = INF;
    }

    dfs(1);

    for (int i = 0; i <= n; i++)
    {
        cout << best[i] << '\n';
    }

    return 0;
}