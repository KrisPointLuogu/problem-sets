#include <bits/stdc++.h>
using namespace std;

const long long MAXN = 2e5 + 5;
const long long INF = 1000000000;

long long n;
long long a[MAXN];
long long arrmax = 0;
vector<long long> pos[MAXN];

bool subtask_4 = 1;

void init()
{
    set<long long> s2;
    set<long long> s;
    cin >> n;
    for (long long i = 1; i <= n; i++)
    {
        cin >> a[i];
        s.insert(a[i]);
        arrmax = max(1ll * a[i], arrmax);
        pos[a[i]].push_back(i);
    }
    for (int i = 0; i < n; i++)
    {
        s2.insert(i);
    }
    if (s != s2)
        subtask_4 = 0;
}

long long get_from_0()
{
    long long prev = 0;
    long long val = 0;
    for (auto it : pos[0])
    {
        long long diff = it - prev - 1;

        const long long len = diff;
        const long long sum = len * (len + 1) / 2;
        val += sum;

        prev = it;
    }
    const long long tot_len = n;
    const long long last_len = tot_len - prev;
    const long long sum = last_len * (last_len + 1) / 2;
    val += sum;
    return val;
}

long long minL, minR;

bool get_cover_from_others(long long X)
{
    // init
    minL = n + 1;
    minR = 0;
    // work
    for (long long i = 0; i < X; i++)
    {
        vector<long long> &ipos = pos[i];
        if (ipos.empty())
            return 0;
        for (auto it : ipos)
        {
            long long pos = it;
            minR = max(pos, minR);
            minL = min(pos, minL);
        }
    }
    return 1;
}
long long prev_x, next_x;
bool get_x_not_in_minl_to_minr(long long X)
{
    prev_x = 0;
    next_x = n + 1;

    for (auto it : pos[X])
    {
        if (minL <= it && it <= minR)
            return 0;
        if (it < minL)
            prev_x = max(it, prev_x);
        if (it > minR)
            next_x = min(it, next_x);
    }

    return 1;
}

void work()
{
    if (pos[0].empty())
    {
        cout << 0 << endl;
        return;
    }

    long long val = 0;
    val += get_from_0();
    for (long long i = 1; i <= arrmax; i++)
    {
        if (pos[i].empty())
            break;
        if (!get_cover_from_others(i))
            break;
        if (!get_x_not_in_minl_to_minr(i))
            continue;

        val += (minL - prev_x) * (next_x - minR);
    }

    cout << val << endl;
}

void work_4()
{
    if (pos[0].empty())
    {
        cout << 0 << endl;
        return;
    }

    long long val = 0;
    val += get_from_0();
    minL = pos[0][0];
    minR = pos[0][0];
    for (long long i = 1; i <= arrmax; i++)
    {
        if (pos[i].empty())
            break;
        if (!get_x_not_in_minl_to_minr(i))
            continue;
        val += (minL - prev_x) * (next_x - minR);
        minL = min(minL, pos[i][0]);
        minR = max(minR, pos[i][0]);
    }
    cout << val << endl;
}

int main()
{
    init();
    if (subtask_4)
        work_4();
    else
        work();
    return 0;
}