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

void work()
{
    sort(tasks + 1, tasks + 1 + n, task_cmp);
    int nowtime = 0;
    int val = 0;
    for (int i = 1; i <= n; i++)
    {
        if (check_task(i, nowtime))
        {
            val += tasks[i].p;
            nowtime++;
        }
    }
    cout << val << endl;
}

int main()
{
    init();
    work();
    return 0;
}
