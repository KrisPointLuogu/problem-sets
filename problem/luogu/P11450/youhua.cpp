#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;

int n, q;
int left_xy[MAXN][MAXN];
int left_xz[MAXN][MAXN];
int left_yz[MAXN][MAXN];
long long ans;

/*
notes : 把三维代码转成三个二维数组
巧
*/

int main()
{
    cin >> n >> q;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            left_xy[i][j] = n;
            left_xz[i][j] = n;
            left_yz[i][j] = n;
        }
    }

    while (q--)
    {
        int x, y, z;
        cin >> x >> y >> z;

        left_xy[x][y]--;
        if (left_xy[x][y] == 0)
            ans++;

        left_xz[x][z]--;
        if (left_xz[x][z] == 0)
            ans++;

        left_yz[y][z]--;
        if (left_yz[y][z] == 0)
            ans++;

        cout << ans << '\n';
    }

    return 0;
}