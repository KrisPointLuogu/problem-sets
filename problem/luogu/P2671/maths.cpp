#include <bits/stdc++.h>
using namespace std;

const int mod = 10007;
int n, m, a[100005], c[100005];
vector<pair<int, int>> v1[100005], v2[100005];
int ans = 0;

void Read()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++)
        scanf("%d", &c[i]);
}

// 分离数组
void Prefix()
{
    for (int i = 1; i <= n; i++)
    {
        if (i & 1)
            v1[c[i]].push_back(make_pair(a[i], i));
        else
            v2[c[i]].push_back(make_pair(a[i], i));
    }
}

void Solve1()
{
    for (int i = 1; i <= m; i++)
    {
        int siz = v1[i].size();
        if (siz < 2)
            continue; // 零个或一个元素的贡献显然为0
        int ans1 = 0, ans2 = 0;
        // 算上述公式
        for (int j = 0; j < siz; j++)
        {
            ans1 += v1[i][j].first;
            ans1 %= mod;
            ans2 += v1[i][j].second;
            ans2 %= mod;
        }
        ans += ans1 * ans2 % mod;
        ans %= mod;
        for (int j = 0; j < siz; j++)
        {
            int cur = (siz - 2) % mod * v1[i][j].second % mod;
            ans += cur % mod * v1[i][j].first;
            ans %= mod;
        }
    }
}

// 与Solve1()同理
void Solve2()
{
    for (int i = 1; i <= m; i++)
    {
        int siz = v2[i].size();
        if (siz < 2)
            continue;
        int ans1 = 0, ans2 = 0;
        for (int j = 0; j < siz; j++)
        {
            ans1 += v2[i][j].first;
            ans1 %= mod;
            ans2 += v2[i][j].second;
            ans2 %= mod;
        }
        ans += ans1 * ans2 % mod;
        ans %= mod;
        for (int j = 0; j < siz; j++)
        {
            int cur = (siz - 2) % mod * v2[i][j].second % mod;
            ans += cur % mod * v2[i][j].first;
            ans %= mod;
        }
    }
}

int main()
{
    Read();
    Prefix();
    Solve1();
    Solve2();
    printf("%d", ans);
    return 0;
}
