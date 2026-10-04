#include <bits/stdc++.h>
using namespace std;

struct Point
{
    int x, y;
} points[100005];

int n, k;

bool isdis1(Point a, Point b)
{
    if (abs(b.x - a.x) == 1 && b.y == a.y)
        return true;
    if (abs(b.y - a.y) == 1 && b.x == a.x)
        return true;
    return false;
}

int dp[100005];
void kequ0dp()
{
    int maxans = -1;
    for (int i = 1; i <= n; i++)
    {
        dp[i] = 1;
        for (int j = 1; j < i; j++)
        {
            if (isdis1(points[j], points[i])) // j < i
                dp[i] = max(dp[i], dp[j] + 1);
        }
        maxans = max(maxans, dp[i]);
    }
    cout << maxans << endl;
}

int main(int argc, char const *argv[])
{
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
    {
        cin >> points[i].x >> points[i].y;
    }

    sort(points + 1, points + 1 + n, [](Point &a, Point &b)
         {
        if(a.x == b.x) return a.y < b.y;
        return a.x < b.x; });

    if (k == 0)
        kequ0dp();

    return 0;
}
