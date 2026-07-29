#include <bits/stdc++.h>
using namespace std;

const int maxn = 2e6 + 5;
typedef pair<int, int> P;

int n, q;
P a[maxn]; // first : content , second : id
P b[maxn];
bool cmp(P a, P b)
{
    return a.first < b.first;
}

int main()
{
    cin >> n >> q;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i].first;
        a[i].second = i;
    }

    // stable_sort(a + 1, a + 1 + n, cmp);

    while (q--)
    {
        int op, x, y;
        cin >> op >> x;
        if (op == 1)
        {
            cin >> y;
            a[x].first = y;
        }
        if (op == 2)
        {
            copy(a, a + maxn, b);
            stable_sort(b + 1, b + 1 + n, cmp);
            for (int i = 1; i <= n; i++)
            {
                if (b[i].second == x)
                {
                    cout << i << endl;
                    break;
                }
            }
        }
    }

    return 0;
}
