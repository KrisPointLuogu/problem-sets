#include <bits/stdc++.h>
using namespace std;

int x, y, z;

int main(int argc, char const *argv[])
{
    cin >> x >> y >> z;

    long long ans = 0;
    for (int i = x; i <= y; i++)
    {
        ans += i / z;
    }

    cout << ans << endl;

    return 0;
}
