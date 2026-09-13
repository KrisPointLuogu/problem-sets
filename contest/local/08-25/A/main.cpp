#include <bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int t;
    int max_x = INT32_MIN, max_y = INT32_MIN;
    int min_x = INT32_MAX, min_y = INT32_MAX;

    cin >> t;
    while (t--)
    {
        int x, y;
        cin >> x >> y;
        max_x = max(x, max_x);
        max_y = max(y, max_y);
        min_x = min(x, min_x);
        min_y = min(y, min_y);
    }

    max_x++;
    max_y++;
    min_x--;
    min_y--;
    cout << min_x << " " << min_y << endl;
    cout << max_x << " " << max_y << endl;

    return 0;
}
