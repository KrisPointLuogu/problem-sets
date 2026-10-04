#include <bits/stdc++.h>
using namespace std;

long long k;

long long ispow2(long long num)
{
    long long b = floor(sqrt(num));
    long long c = ceil(sqrt(num));
    return b * b == num && c * c == num && c == b;
}

int main(long long argc, char const *argv[])
{
    cin >> k;
    while (k--)
    {
        long long n, d, e;
        cin >> n >> d >> e;
        long long m = n - e * d + 2;
        long long delta = m * m - 4 * n;
        if (delta < 0 || !ispow2(delta))
        {
            cout << "NO" << endl;
            continue;
        }

        long long p2 = m + sqrt(delta);
        long long p = p2 / 2;
        long long q = n / p;
        if (p > q)
            swap(p, q);
        if (p <= 0 || q <= 0)
        {
            cout << "NO" << endl;
            continue;
        }
        cout << p << " " << q << endl;
    }
    return 0;
}
