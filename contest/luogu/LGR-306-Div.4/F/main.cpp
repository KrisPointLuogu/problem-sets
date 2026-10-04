#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6;

vector<long long> cur(maxn), t(maxn);
vector<vector<pair<int, long long>>> adj(maxn);
int main(int argc, char const *argv[])
{
    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
    {
        cin >> cur[i];
    }
    for (int i = 1; i <= n; i++)
    {
        cin >> t[i];
    }

    for (int i = 1; i <= n - 1; i++)
    {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        adj[u].push_back({v, c});
        adj[v].push_back({u, c});
    }

    vector<int> p(n + 1, 0), order;
    vector<long long> c(n + 1, 0), s(n + 1, 0);
    order.reserve(n);

    vector<int> sta;
    sta.push_back(1);
    p[1] = -1;
    while (!sta.empty())
    {
        int u = sta.back();
        sta.pop_back();
        order.push_back(u);
        for (auto edge : adj[u])
        {
            if (edge.first == p[u])
            {
                continue;
            }
            p[edge.first] = u;
            c[edge.first] = edge.second;
            sta.push_back(edge.first);
        }
    }

    for (int i = 1; i <= n; i++)
    {
        s[i] = cur[i] - t[i];
    }

    long long ans = 0;
    for (int i = n; i >= 1; i--)
    {
        int u = order[i - 1];
        if (p[u] == -1)
        {
            continue;
        }
        long long flow = s[u];
        if (flow < 0)
        {
            flow = -flow;
        }
        ans += (flow + c[u] - 1) / c[u];
        s[p[u]] += s[u];
    }

    cout << ans << endl;
    return 0;
}
