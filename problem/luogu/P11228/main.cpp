#include <bits/stdc++.h>
using namespace std;

int t;
int n, m, k;
int x, y, d;
bool a[1005][10005];
bool vis[1005][10005];

void init()
{
    cin >> n >> m >> k;
    cin >> x >> y >> d;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            char c;
            cin >> c;
            if (c == '.')
                a[i][j] = 1;
        }
    }
}

int dx[4]{0, 1, 0, -1};
int dy[4]{1, 0, -1, 0};

bool check(int x, int y)
{
    if (x < 1 || y < 1)
        return false;

    if (x > n || y > m)
        return false;

    if (!a[x][y])
        return false;

    return true;
}

void work()
{
    int cnt = 0;
    vis[x][y] = 1;
    while (k--)
    {
        int nx = x + dx[d];
        int ny = y + dy[d];
        // if(a[nx][ny]) cnt++;
        // printf("x:%d y:%d dir:%d     map[x][y]:%d map[nx][ny]:%d\n",x,y,d,a[x][y],a[nx][ny]);
        if (!check(nx, ny))
        {
            d = (d + 1) % 4;
            continue;
        }
        x = nx;
        y = ny;
        vis[x][y] = 1;
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (vis[i][j])
                cnt++;
        }
    }

    cout << cnt << endl;
}

int main(int argc, char const *argv[])
{
    cin >> t;
    while (t--)
    {
        memset(a, 0, sizeof(a));
        memset(vis, 0, sizeof(vis));
        x = y = d = 0;
        n = m = k = 0;
        init();
        work();
    }
    return 0;
}
