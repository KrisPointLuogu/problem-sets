#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e3 + 5;
int mat[5][maxn];
long long v[5][maxn];
int n;
long long ans = INT32_MIN;

enum COLOR
{
    X = 0,
    O = 1
};

bool is_in_cond(int x, int y, int color)
{
    int dir[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (int d = 0; d < 4; d++)
    {
        int dx = dir[d][0], dy = dir[d][1];
        for (int start = -2; start <= 0; start++)
        {
            int x1 = x + start * dx, y1 = y + start * dy;
            int x2 = x1 + dx, y2 = y1 + dy;
            int x3 = x2 + dx, y3 = y2 + dy;
            if (x1 >= 1 && x1 <= 3 && y1 >= 1 && y1 <= n &&
                x2 >= 1 && x2 <= 3 && y2 >= 1 && y2 <= n &&
                x3 >= 1 && x3 <= 3 && y3 >= 1 && y3 <= n)
            {
                if (mat[x1][y1] == color && mat[x2][y2] == color && mat[x3][y3] == color)
                {

                    if ((x == x1 && y == y1) || (x == x2 && y == y2) || (x == x3 && y == y3))
                        return true;
                }
            }
        }
    }
    return false;
}

void dfs(int dep)
{
    if (dep >= 3 * n)
    {
        long long red = 0, blue = 0;
        for (int i = 1; i <= 3; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (is_in_cond(i, j, mat[i][j]))
                {
                    if (mat[i][j] == X)
                        red += v[i][j];
                    else
                        blue += v[i][j];
                }
            }
        }
        long long sum = red - blue;
        ans = max(ans, sum);
        return;
    }
    int div = dep / n + 1; // 行号 1~3
    int m = dep % n + 1;   // 列号 1~n
    for (int color = 0; color <= 1; color++)
    {
        mat[div][m] = color;
        dfs(dep + 1);
    }
}

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cin >> v[i][j];
        }
    }
    dfs(0);
    cout << ans << endl;
    return 0;
}