#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

long long n;
struct task
{
    long long d, p;
    // first
    // second
} tasks[maxn];

bool task_cmp(task A, task B)
{
    if (A.d == B.d)
    {
        return A.p > B.p;
    }
    return A.d < B.d;
}

void init()
{
    cin >> n;
    for (long long i = 1; i <= n; i++)
    {
        cin >> tasks[i].d >> tasks[i].p;
    }
}

void debug()
{
    for (long long i = 1; i <= n; i++)
    {
        cout << tasks[i].d << " " << tasks[i].p << endl;
    }
    cout << endl
         << endl;
}

bool check_task(long long idx, long long nowtime)
{
    return tasks[idx].d > nowtime;
}

void work_baoli()
{
    long long maxval = -1;
    sort(tasks + 1, tasks + 1 + n, task_cmp);
    do
    {
        long long now_time = 0;
        long long val = 0;
        for (long long i = 1; i <= n; i++)
        {
            if (check_task(i, now_time))
            {
                val += tasks[i].p;
                now_time++;
            }
        }
        maxval = max(maxval, val);
    } while (next_permutation(tasks + 1, tasks + 1 + n, task_cmp));
    cout << maxval << endl;
}

int main()
{
    init();
    work_baoli();
    return 0;
}
