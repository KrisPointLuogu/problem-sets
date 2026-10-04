#include <bits/stdc++.h>
using namespace std;

int n, d;
struct island
{
    int x, y;
    double r;
} islands[1005];

void calc(island &a)
{
    a.r = a.x + sqrt(1.0 * d * d - 1.0 * a.y * a.y);
}

bool check(double cx, double cy, island p)
{
    double dx = cx - p.x, dy = cy - p.y;
    return dx * dx + dy * dy <= 1.0 * d * d + 1e-9;
}

int main(int argc, char const *argv[])
{
    cin >> n >> d;
    for (int i = 1; i <= n; i++)
    {
        cin >> islands[i].x >> islands[i].y;
        if (islands[i].y > d)
        {
            cout << -1 << endl;
            return 0;
        }
        calc(islands[i]);
    }

    sort(islands + 1, islands + 1 + n, [](island &a, island &b)
         { return a.r < b.r; });

    vector<double> ldsum;

    for (int i = 1; i <= n; i++)
    {
        bool flag = true;
        for (auto ld : ldsum)
        {
            if (check(ld, 0, islands[i]))
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            ldsum.push_back(islands[i].r);
        }
    }

    cout << ldsum.size() << endl;

    return 0;
}
