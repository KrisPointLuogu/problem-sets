#include <bits/stdc++.h>
using namespace std;

int T;

int n, k;
string A;

void init()
{
    cin >> n >> k;
    cin >> A;
}

void work()
{
    int maxans = -1;
    int x = 0, y = 0;
    while (k--)
    {
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
            int zh = abs(x) + abs(y);
            maxans = max(maxans, zh);
            cout << c << " " << x << " " << y << " " << zh << " " << maxans << endl;
        }
    }
    cout << maxans << endl;
}

int main(int argc, char const *argv[])
{
    cin >> T;
    while (T--)
    {
        init();
        work();
    }
    return 0;
}
