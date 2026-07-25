#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;

int n, q;
bool removed_block[MAXN][MAXN][MAXN];

bool empty_x_line(int y, int z)
{
    for (int x = 0; x < n; x++)
    {
        if (!removed_block[x][y][z])
            return false;
    }
    return true;
}

bool empty_y_line(int x, int z)
{
    for (int y = 0; y < n; y++)
    {
        if (!removed_block[x][y][z])
            return false;
    }
    return true;
}

bool empty_z_line(int x, int y)
{
    for (int z = 0; z < n; z++)
    {
        if (!removed_block[x][y][z])
            return false;
    }
    return true;
}

long long calc_answer()
{
    long long ans = 0;

    for (int y = 0; y < n; y++)
    {
        for (int z = 0; z < n; z++)
        {
            if (empty_x_line(y, z))
                ans++;
        }
    }

    for (int x = 0; x < n; x++)
    {
        for (int z = 0; z < n; z++)
        {
            if (empty_y_line(x, z))
                ans++;
        }
    }

    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            if (empty_z_line(x, y))
                ans++;
        }
    }

    return ans;
}

int main()
{
    cin >> n >> q;

    while (q--)
    {
        int x, y, z;
        cin >> x >> y >> z;

        removed_block[x][y][z] = true;
        cout << calc_answer() << '\n';
    }

    return 0;
}