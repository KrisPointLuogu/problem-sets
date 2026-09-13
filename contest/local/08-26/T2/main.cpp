#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;

long long n, m;

struct book
{
    long long a, b;
    long long d;
} bk[maxn];

bool cmp(book a, book b)
{
    return a.d > b.d;
}

int main(long long argc, char const *argv[])
{
    cin >> n >> m;
    for (long long i = 1; i <= n; i++)
    {
        cin >> bk[i].a;
    }
    for (long long i = 1; i <= n; i++)
    {
        cin >> bk[i].b;
        bk[i].d = bk[i].a - bk[i].b;
    }

    sort(bk + 1, bk + 1 + n, cmp);

    // for (long long i = 1; i <= n; i++)
    // {
    //     cout << (bk[i].take ? "TRUE " : "FALSE ") << bk[i].a << " " << bk[i].b << endl;
    // }

    long long cnt = 0;
    long long ans = 0;
    for (long long i = 1; i <= n; i++)
    {
        if (cnt < m && bk[i].d > 0)
        {
            ans += bk[i].d;
            cnt++;
        }
        ans += bk[i].b;
    }

    cout << ans << endl;

    return 0;
}
