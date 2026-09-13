#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int n;
int a[maxn];
int m;
int l[maxn], r[maxn];
bool flag_task2 = 1;

// int b[55];
vector<int> b(55, -1);

void init()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        b[a[i]]++;
        if (a[i] > 2)
            flag_task2 = 0;
    }

    cin >> m;
    for (int i = 1; i <= m; i++)
    {
        cin >> l[i] >> r[i];
    }
}

vector<int> tmpv;

void work(int idx)
{
    tmpv.clear();
    int nl = l[idx];
    int nr = r[idx];

    for (int i = nl; i <= nr; i++)
    {
        tmpv.push_back(a[i]);
    }

    sort(tmpv.begin(), tmpv.end());

    int ans = 0;

    for (int i = 1; i < tmpv.size(); i++)
    {
        if (tmpv[i] - tmpv[i - 1] >= 2)
            break;
        ans++;
    }
    cout << ans + 1 << endl;
}

void work_task2(int idx)
{
    int nl = l[idx];
    int nr = r[idx];
    cout << nr - nl + 1 << endl;
}

int main(int argc, char const *argv[])
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    init();

    if (flag_task2)
    {
        for (int i = 1; i <= m; i++)
        {
            work_task2(i);
        }
    }
    else
    {
        for (int i = 1; i <= m; i++)
        {
            work(i);
        }
    }

    return 0;
}
