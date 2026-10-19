#include <bits/stdc++.h>
using namespace std;

int n, m, k;

vector<vector<int>> adj;
map<pair<int, int>, int> adj_w;

void add_edge(int u, int v, int w = 0)
{
    adj[u].push_back(v);
    adj_w[make_pair(u, v)] = w;
}

int get_adj_w(int u, int v)
{
    return adj_w[make_pair(u, v)];
}

int dis[1000005];

int main(int argc, char const *argv[])
{

    cin >> n >> m >> k;
    for (int i = 1; i <= m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
    }

    for (auto vi : adj)
    {
        sort(vi.begin(), vi.end());
    }

    for (int i = 1; i <= n; i++)
    {
        dis[i] = -1;
    }

    queue<int> nodes;
    nodes.push(1);

    return 0;
}
