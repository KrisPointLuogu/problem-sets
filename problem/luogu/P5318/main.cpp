#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll n, m;
vector<vector<int>> g;
vector<int> vis;

void read_input()
{
    cin >> n >> m;

    g.assign(n + 1, vector<int>());
    for (ll i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }
}

void build_graph()
{
    for (ll u = 1; u <= n; u++)
    {
        sort(g[u].begin(), g[u].end());
    }
}

void dfs_solve()
{
    vis.assign(n + 1, 0);

    stack<int> st;
    st.push(1);

    while (!st.empty())
    {
        int u = st.top();
        st.pop();
        if (vis[u] == 1)
        {
            continue;
        }
        vis[u] = 1;
        cout << u << ' ';

        ll deg = g[u].size();
        for (ll i = deg - 1; i >= 0; i--)
        {
            int v = g[u][i];
            if (vis[v] == 0)
            {
                st.push(v);
            }
        }
    }
    cout << '\n';
}

void bfs_solve()
{
    vis.assign(n + 1, 0);

    queue<int> q;
    q.push(1);
    vis[1] = 1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        cout << u << ' ';

        ll deg = g[u].size();
        for (ll i = 0; i < deg; i++)
        {
            int v = g[u][i];
            if (vis[v] == 0)
            {
                vis[v] = 1;
                q.push(v);
            }
        }
    }
    cout << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    read_input();
    build_graph();
    dfs_solve();
    bfs_solve();

    return 0;
}