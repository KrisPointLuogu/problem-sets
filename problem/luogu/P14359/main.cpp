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

int main(int argc, char const *argv[])
{
    cin >> n >> k;

    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        b[i] = b[i - 1] ^ a[i];
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            if (pq.empty())
                goto __first;
            if (pq.top() >= j)
            {
                // cout << "BREAK: " << pq.top() << " " << j << endl;
                continue;
            }
        __first:

            int sum = get_lr(j, i);
            if (sum == k)
            {
                // cout << i << " " << j << endl;
                pq.push(i);
                pq.push(j);
                ans++;
            }
        }
    }

    cout << ans << endl;

    return 0;
}
