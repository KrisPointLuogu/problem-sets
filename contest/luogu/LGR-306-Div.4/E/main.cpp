#include <bits/stdc++.h>
using namespace std;

int H, W;
int a[35][35];

int calc(int x, int y, int c)
{
    int old = a[x][y];
    a[x][y] = c;
    int cnt = 0;
    for (int i = 1; i <= H; i++)
    {
        for (int j = 1; j < W; j++)
        {
            if (a[i][j] == a[i][j + 1])
            {
                cnt++;
            }
        }
    }
    for (int i = 1; i < H; i++)
    {
        for (int j = 1; j <= W; j++)
        {
            if (a[i][j] == a[i + 1][j])
            {
                cnt++;
            }
        }
    }
    a[x][y] = old;
    return cnt;
}

int main(int argc, char const *argv[])
{
    cin >> H >> W;
    for (int i = 1; i <= H; i++)
    {
        for (int j = 1; j <= W; j++)
        {
            cin >> a[i][j];
        }
    }

    int ans = calc(1, 1, a[1][1]);
    for (int i = 1; i <= H; i++)
    {
        for (int j = 1; j <= W; j++)
        {
            for (int c = 1; c <= 3; c++)
            {
                ans = max(ans, calc(i, j, c));
            }
        }
    }

    cout << ans << endl;
    return 0;
}
