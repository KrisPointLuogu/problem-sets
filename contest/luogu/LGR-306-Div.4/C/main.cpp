#include <bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int n, r;
    cin >> n >> r;
    int c = (n + 1) / 2;
    for (int x = 1; x <= n; x++)
    {
        for (int y = 1; y <= n; y++)
        {
            int dx = x - c, dy = y - c;
            if (dx * dx + dy * dy <= r * r)
                cout << '#';
            else
                cout << '.';
        }
        cout << endl;
    }
    return 0;
}
