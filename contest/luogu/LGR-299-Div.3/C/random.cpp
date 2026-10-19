#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

// 生成 [l, r] 之间的随机整数。
int rnd(int l, int r)
{
    return uniform_int_distribution<int>(l, r)(rng);
}

long long rndll(long long l, long long r)
{
    return uniform_int_distribution<long long>(l, r)(rng);
}

// 将 total 个单位分配到 cnt 个槽位，每个值在 [lo, hi] 内且总和等于 total。
vector<long long> distribute(int cnt, long long total, long long lo, long long hi)
{
    vector<long long> v(cnt, lo);
    long long rem = total - (long long)cnt * lo;
    long long cap = (long long)cnt * (hi - lo);
    if (rem > cap)
        rem = cap;
    while (rem > 0)
    {
        int idx = rnd(0, cnt - 1);
        if (v[idx] < hi)
        {
            v[idx]++;
            rem--;
        }
    }
    return v;
}

// 生成子任务 4 数据（a_i = b_i）。
// 用法：./random            —— 随机生成小样例（n <= 10）
//       ./random [n] [q] [k] —— 指定规模
int main(int argc, char **argv)
{
    int n = 0, q = 0, k = 0;
    if (argc > 1)
    {
        n = atoi(argv[1]);
        q = atoi(argv[2]);
        k = atoi(argv[3]);
        n = min(max(n, 1), 250);
        q = min(max(q, 1), 250);
    }
    else
    {
        n = rnd(1, 10);
        q = rnd(1, 10);
        k = rnd(0, n);
    }
    k = min({k, n, 30});

    long long x = rnd(1, 250);

    // 随机挑 k 台作为特殊兑换机
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    vector<int> special(n, 0);
    for (int i = 0; i < k; i++)
        special[order[i]] = 1;

    // a_i = b_i，需保证 sum a_i = sum b_i <= 2.5e5
    long long sum_a = rndll(n, min(250LL, 20LL * n));
    vector<long long> a = distribute(n, sum_a, 1, 20);

    // c_i 总和不超过 2.5e5（仅特殊兑换机）
    long long sum_c = rndll(k, 250);
    vector<long long> c_all = distribute(k, sum_c, 1, 250);

    cout << n << " " << x << " " << q << "\n";
    int ci = 0;
    for (int i = 0; i < n; i++)
    {
        cout << special[i] << " " << a[i] << " " << a[i];
        if (special[i])
            cout << " " << c_all[ci++];
        cout << "\n";
    }
    for (int i = 0; i < q; i++)
    {
        // 一半小值，一半可到 1e18
        long long y = rndll(1, 10LL);
        cout << y << "\n";
    }
    return 0;
}
