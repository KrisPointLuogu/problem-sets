#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;

long long n;
long long c[maxn];

long long m;
long long p1, s1, s2;

long long dragon_sum;
long long tiger_sum;

long long sum[maxn];

auto myabs = [](long long x)
{ return (x < 0) ? -x : x; };
void pre()
{

    for (long long i = 1; i <= n; i++)
    {
        if (i == m)
        {
            continue;
        }
        long long k = myabs(i - m);
        long long nc = c[i];
        long long mul = k * nc;
        sum[i] = mul;
        if (i < m)
        {
            dragon_sum += mul;
        }
        else if (i > m)
        {
            tiger_sum += mul;
        }
    }
}

void init()
{
    cin >> n;
    for (long long i = 1; i <= n; i++)
    {
        cin >> c[i];
    }
    cin >> m;
    cin >> p1 >> s1 >> s2;
}

long long minans = LONG_LONG_MAX;
long long pos;

void work()
{
    c[p1] += s1;
    pre();

    // cout << dragon_sum << endl;
    // cout << tiger_sum << endl;

    for (long long i = 1; i <= n; i++)
    {
        long long now_select_p = i; // p2
        long long my_sodier_sum = s2;

        c[now_select_p] += my_sodier_sum;
        if (i < m)
            dragon_sum -= sum[i];
        else if (i > m)
            tiger_sum -= sum[i];

        long long k = myabs(now_select_p - m);
        long long nc = c[now_select_p];
        long long mul = k * nc;
        if (i < m)
        {
            dragon_sum += mul;
        }
        else if (i > m)
        {
            tiger_sum += mul;
        }

        long long ans = myabs(dragon_sum - tiger_sum);
        if (minans > ans)
        {
            minans = ans;
            pos = i;
        }

        if (i < m)
        {
            dragon_sum -= mul;
        }
        else if (i > m)
        {
            tiger_sum -= mul;
        }
        if (i < m)
            dragon_sum += sum[i];
        else if (i > m)
            tiger_sum += sum[i];
        c[now_select_p] -= my_sodier_sum;
    }

    cout << pos << endl;
}

int main()
{
    init();
    work();
    return 0;
}
