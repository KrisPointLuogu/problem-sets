#include <bits/stdc++.h>
using namespace std;

int T;

int n;
long long k;
string A;

void init()
{
    cin >> n >> k;
    cin >> A;
}

void work()
{
    long long x = 0, y = 0;
    long long dx = 0, dy = 0;

    for (auto c : A)
    {
        if (c == 'U')
            y++;
        if (c == 'D')
            y--;
        if (c == 'L')
            x--;
        if (c == 'R')
            x++;
    }
    dx = x;
    dy = y;

    long long ans = 0;

    x = y = 0;
    for (auto c : A)
    {
        if (c == 'U')
            y++;
        if (c == 'D')
            y--;
        if (c == 'L')
            x--;
        if (c == 'R')
            x++;
        ans = max(ans, abs(x) + abs(y));
        ans = max(ans, abs((k - 1) * dx + x) + abs((k - 1) * dy + y));
    }

    cout << ans << endl;
}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> T;
    while (T--)
    {
        init();
        work();
    }
    return 0;
}
