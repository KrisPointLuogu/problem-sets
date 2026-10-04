#include <bits/stdc++.h>
using namespace std;

int n, k;
const int maxn = 1e6 + 5;

int a[maxn];
int b[maxn];
priority_queue<int> pq;

int get_lr(int l, int r)
{
    return (b[r] ^ b[l - 1]);
}
int ans = 0;
void work_simple()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            if (pq.empty())
                goto __first;
            if (pq.top() >= j)
            {
                continue;
            }
        __first:

            int sum = get_lr(j, i);
            if (sum == k)
            {
                pq.push(i);
                pq.push(j);
                ans++;
            }
        }
    }
}

bool flag_B = 1;

int c[maxn];

int get_c_sum(int l, int r)
{
    return c[r] - c[l - 1];
}

void work_B()
{
    if (k > 1)
        return;
    bool seen[2] = {true, false};
    int cur = 0;
    for (int i = 1; i <= n; i++)
    {
        cur ^= a[i];
        if (seen[cur ^ k])
        {
            ans++;
            seen[0] = seen[1] = false;
            seen[cur] = true;
        }
        else
            seen[cur] = true;
    }
}

int main(int argc, char const *argv[])
{
    cin >> n >> k;

    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        b[i] = b[i - 1] ^ a[i];
        c[i] = c[i - 1] + a[i];
        if (a[i] != 1 && a[i] != 0)
            flag_B = 0;
    }
    if (flag_B)
        work_B();
    else
        work_simple();

    cout << ans << endl;

    return 0;
}
