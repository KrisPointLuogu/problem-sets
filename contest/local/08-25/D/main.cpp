#include <bits/stdc++.h>
using namespace std;

const int maxn = 505;

int n;
int a[maxn], b[maxn];

int main(int argc, char const *argv[])
{
    cin >> n;
    int max = 0;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i] >> b[i];
        max = std::max(max, a[i] + b[i]);
    }

    cout << max << endl;

    return 0;
}
