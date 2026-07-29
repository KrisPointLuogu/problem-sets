#include <bits/stdc++.h>
using namespace std;

const int maxn = 8005;
typedef pair<int, int> P;

int n, q;
P a[maxn];
int pos[maxn];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    cin >> n >> q;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i].first;
        a[i].second = i;
    }

    stable_sort(a + 1, a + 1 + n);

    for (int i = 1; i <= n; i++)
    {
        pos[a[i].second] = i;
    }

    while (q--)
    {
        int op, x, v;
        cin >> op >> x;
        if (op == 1)
        {
            cin >> v;
            for (int i = 1; i <= n; i++)
            {
                if (a[i].second == x)
                {
                    a[i].first = v;
                    break;
                }
            }
            stable_sort(a + 1, a + 1 + n);
            for (int i = 1; i <= n; i++)
            {
                pos[a[i].second] = i;
            }
        }
        else
        {
            cout << pos[x] << '\n';
        }
    }

    return 0;
}
