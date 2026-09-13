#include <bits/stdc++.h>
using namespace std;

const int maxn = 100 + 5;

string W;
int R, C;

char ma[maxn][maxn];

int ans;

int dx[8]{0, 1, 1, 1, 0, -1, -1, -1};
int dy[8]{1, 1, 0, -1, -1, -1, 0, 1};

int vis[maxn][maxn];
bool check(int x, int y, char need_match)
{
    if (!(1 <= x && x <= R))
        return 0;
    if (!(1 <= y && y <= C))
        return 0;
    // if (vis[x][y])
    // return 0;
    if (ma[x][y] != need_match)
        return 0;
    return 1;
}

bool get_ver(int ai, int bi)
{
    return dx[ai] * dx[bi] + dy[ai] * dy[bi] == 0;
}

void dfs(int nowx, int nowy, int stringpos, bool ifturn, int lastidx)
{
    if (stringpos == W.size() - 1)
    {
        ans++;
        return;
    }
    char need_match = W[stringpos + 1];
    for (int i = 0; i < 8; i++)
    {
        int nxtx = nowx + dx[i];
        int nxty = nowy + dy[i];
        if (!check(nxtx, nxty, need_match))
        {
            continue;
        }
        if (lastidx != -1 && i != lastidx && !(get_ver(i, lastidx) && !ifturn))
        {
            continue;
        }
        bool flag = ifturn || (lastidx != -1 && get_ver(i, lastidx));
        dfs(nxtx, nxty, stringpos + 1, flag, i);
    }
}

vector<pair<int, int>> vp;

int main(int argc, char const *argv[])
{
    cin >> W;
    cin >> R >> C;
    for (int i = 1; i <= R; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            cin >> ma[i][j];
            if (ma[i][j] == W[0])
                vp.push_back(make_pair(i, j));
        }
    }

    for (auto p : vp)
    {
        int i = p.first;
        int j = p.second;
        memset(vis, 0, sizeof vis);
        dfs(i, j, 0, false, -1);
    }

    cout << ans << endl;

    return 0;
}
