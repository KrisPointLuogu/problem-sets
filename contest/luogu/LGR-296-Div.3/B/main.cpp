#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

long long n;
long long a[maxn];
long long cube[maxn];
long long D[maxn];
long long out[maxn];

int main()
{
    cin >> n;
    for (long long i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    for (long long i = 1; i * i * i <= n; i++)
    {
        cube[i * i * i] = true;
    }

    //  k section = [k^3, (k+1)^3-1]，point : c = k
    long long t = 0;
    for (long long k = 1; k * k * k <= n; k++)
    {
        long long c = (long long)k;
        long long lo = k * k * k;
        long long hi = min((long long)n, (k + 1) * (k + 1) * (k + 1) - 1);
        for (long long p = (long long)lo; p <= hi; p++)
        {
            D[p] = cube[p] ? 0 : 1 + D[p - c];
            t = max(D[p], t);
        }
    }

    vector<long long> cnt(t + 1, 0);
    for (long long p = 1; p <= n; p++)
    {
        cnt[D[p]]++;
    }
    vector<long long> start(t + 1, 0), pos(t + 1, 0);
    long long cur = 1;
    for (long long r = 0; r <= t; r++)
    {
        start[r] = cur;
        pos[r] = cur;
        cur += cnt[r];
    }
    for (long long p = 1; p <= n; p++)
    {
        out[pos[D[p]]++] = a[p];
    }

    cout << t + 1 << endl;
    for (long long r = 0; r <= t; r++)
    {
        for (long long j = 0; j < cnt[r]; j++)
        {
            cout << out[start[r] + j] << " ";
        }
        cout << endl;
    }

    return 0;
}
