#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e6 + 5;
int n;

int a[maxn];

bool good(const vector<int> &d)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        if (d[i] > 0 && d[(i + 1) % n] < 0)
            cnt++;
    }
    return cnt == 1;
}

int bfs()
{
    vector<int> start(n);
    for (int i = 0; i < n; i++)
    {
        start[i] = a[i] - a[(i - 1 + n) % n];
    }
    if (good(start))
        return 0;

    map<vector<int>, int> dist;
    queue<vector<int>> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty())
    {
        auto v = q.front();
        q.pop();
        int di = dist[v];
        for (int i = 0; i < n; i++)
        {
            auto w = v;
            swap(w[i], w[(i + 1) % n]);
            if (dist.count(w))
                continue;
            if (good(w))
                return di + 1;
            dist[w] = di + 1;
            q.push(w);
        }
    }
    return -1;
}

int main()
{
    int T;
    cin >> T;
    while (T--)
    {
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            cin >> a[i];
        }
        cout << bfs() << "\n";
    }
    return 0;
}
