#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e6 + 5;

string s;

int a[maxn];

int main(int argc, char const *argv[])
{
    cin >> s;
    int cnt = 0;
    for (auto c : s)
    {
        if (isdigit(c))
        {
            a[++cnt] = c - '0';
        }
    }

    sort(a + 1, a + 1 + cnt, greater<int>());
    for (int i = 1; i <= cnt; i++)
    {
        cout << a[i];
    }

    return 0;
}
