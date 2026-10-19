#include <bits/stdc++.h>
using namespace std;

const long long maxn = 1e6 + 5;
const long long INF = (1LL << 31) - 1;

vector<pair<long long, long long>> adj[maxn];

long long n, m, s;

void add_edge(long long u, long long v, long long w = 0)
{
    adj[u].push_back({v, w});
}

long long in_queue[maxn];
long long dis[maxn];

int main(int argc, char const *argv[])
{
    cin >> n >> m >> s;
    for (long long i = 1; i <= m; i++)
    {
        long long u, v, w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
    }

    queue<long long> nodes;
    memset(in_queue, 0, sizeof(in_queue));
    fill(dis + 1, dis + 1 + n, INF);
    dis[s] = 0;
    nodes.push(s);
    in_queue[s] = 1;

    while (!nodes.empty())
    {
        long long u = nodes.front();
        nodes.pop();
        in_queue[u] = 0;

        for (auto e : adj[u])
        {
            long long v = e.first;
            long long w = e.second;

            long long cani = dis[u] + w;
            if (cani >= dis[v])
                continue;

            dis[v] = cani;

            if (!in_queue[v])
            {
                nodes.push(v);
                in_queue[v] = 1;
            }
        }
    }

    for (long long i = 1; i <= n; i++)
    {
        cout << dis[i] << " ";
    }
    cout << endl;

    return 0;
}
