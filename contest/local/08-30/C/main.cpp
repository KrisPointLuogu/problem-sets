#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;
int n, p;
int a[maxn];

void task1()
{
    for (int i = 2; i <= n; i++)
    {
        if (a[i - 1] & 1 && a[i] & 1)
        {
                }
    }
}

int main(int argc, char const *argv[])
{
    cin >> n >> p;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }

    if (p == 2)
    {
        task1();
    }

    return 0;
}
