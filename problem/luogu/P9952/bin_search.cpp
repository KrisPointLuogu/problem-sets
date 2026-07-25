#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000005;
int n;
string a;

int mid(int l, int r)
{
    return (l + r) >> 1;
}

bool check(int val)
{
    if (val == 0)
        return false;
    vector<int> pos;
    for (int i = 1; i <= n; i++)
        if (a[i - 1] == '1')
            pos.push_back(i);

    if (pos.empty())
        return val > n - 1;

    for (int i = 1; i < (int)pos.size(); i++)
        if (pos[i] - pos[i - 1] < val)
            return true;

    int cnt = 0;
    cnt += (pos[0] - 1) / val;
    cnt += (n - pos.back()) / val;
    for (int i = 1; i < (int)pos.size(); i++)
    {
        int len = pos[i] - pos[i - 1];
        cnt += max(0, len / val - 1);
    }
    return cnt < 2;
}

int bs_find(int l, int r)
{
    while (l < r)
    {
        int m = mid(l, r);
        if (check(m))
            r = m;
        else
            l = m + 1;
    }
    return l;
}

int main(int argc, char const *argv[])
{
    cin >> n;
    cin >> a;
    int last_pos = -1;
    int d = n + 1;
    for (int i = 1; i <= n; i++)
    {
        int nowa = a[i - 1] - '0';
        if (nowa)
        {
            if (last_pos != -1)
                d = min(d, i - last_pos);
            last_pos = i;
        }
    }

    int ans = bs_find(0, d + 1) - 1;
    cout << ans << endl;

    return 0;
}
