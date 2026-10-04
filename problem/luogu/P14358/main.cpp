#include <bits/stdc++.h>
using namespace std;

int n, m;
const int maxn = 1e3 + 5;

// int a[maxn];

struct stu
{
    int score;
    int R;
} a[maxn];

int main(int argc, char const *argv[])
{
    cin >> n >> m;
    for (int i = 1; i <= n * m; i++)
    {
        cin >> a[i].score;
        a[i].R = (i == 1);
    }

    sort(a + 1, a + 1 + n * m, [](const stu &a, const stu &b)
         { return a.score > b.score; });

    int pos = 0;
    for (int i = 1; i <= n * m; i++)
    {
        if (a[i].R)
        {
            pos = i;
            break;
        }
    }

    int c = (pos - 1) / n + 1;
    int k = (pos - 1) % n;
    int r;
    if (c & 1)
    {
        r = k + 1;
    }
    else
    {
        r = n - k;
    }

    cout << c << " " << r << endl;

    return 0;
}
