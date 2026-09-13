#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int a;
long long n, m;
long long s[maxn];
set<long long> ss;

void init()
{
    memset(s, 0, sizeof s);
    ss.clear();
    cin >> a >> n >> m;
    for (int i = 1; i <= a; i++)
    {
        cin >> s[i]; // s -> a;
        ss.insert(s[i]);
    }
}

void work()
{
    bool ok = true;
    for (auto iter : ss)
    {
        long long now_i = (iter - 1) / m;
        long long now_j = (iter - 1) % m;
        long long vsym = now_i * m + (m - 1 - now_j) + 1;
        if (!ss.count(vsym))
        {
            ok = false;
            break;
        }
    }
    if (ok)
        cout << "Yes" << endl;
    else
        cout << "No" << endl;
}

int main(int argc, char const *argv[])
{
    int T;
    cin >> T;
    while (T--)
    {
        init();
        work();
    }
    return 0;
}
