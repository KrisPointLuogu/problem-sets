#include <bits/stdc++.h>
using namespace std;

enum COLOR
{
    R,
    G,
    B
};

const int maxn = 3e3 + 5;
int n, m;
int a[maxn][maxn];

long long get_hash(int x1, int y1, int x2, int y2)
{
    long long sum = 0;
    for (int i = y1; i <= y2; i++)
    {
        for (int j = x1; j <= x2; j++)
        {
            // i 行 j 列 (y,x)
            sum = sum * 10 + a[i][j];
        }
    }
    return sum;
}

void init()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            char t;
            cin >> t;
            if (t == 'R')
                a[i][j] = R;
            if (t == 'G')
                a[i][j] = G;
            if (t == 'B')
                a[i][j] = B;
        }
    }
}

void work()
{
    int vdiv = n / 3;
    int hdiv = m / 3;
    set<long long> s;
    for (int i = 1; i <= hdiv; i++)
    {
        for (int j = 1; j <= vdiv; j++)
        {
            // i : x , j : y
            int tmp = 0;

            int x1 = (i - 1) * 3 + 1;
            int y1 = (j - 1) * 3 + 1;

            int x2 = i * 3;
            int y2 = j * 3;

            tmp += get_hash(x1, y1, x2, y2);
            s.insert(tmp);
        }
    }

    cout << s.size() << endl;
}

int main(int argc, char const *argv[])
{
    init();
    work();
    return 0;
}
