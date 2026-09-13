#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int n, m;

const int mod = 10007;
int number[maxn], color[maxn];

/*
template<int size>
struct DisjointSet {
    int fa[size+5];
    DisjointSet() {
        for(int i=0;i<=size+4;++i)
            fa[i] = i;
    }
    int find(int u){
        if( u  == fa[u]) return u;
        return fa[u] = find(fa[u]);
    }
    inline void un(int u,int v){
        fa[find(u)] = find(v);
    }
};
*/

int main(int argc, char const *argv[])
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
    {
        cin >> number[i];
    }
    for (int i = 1; i <= n; i++)
    {
        cin >> color[i];
    }
    long long ans = 0;
    for (int x = 1; x <= n; x++)
    {
        for (int y = x + 1; y <= n; y++)
        {
            int z = 2 * y - x;
            if (color[x] != color[z])
            {
                continue;
            }
            long long score = (((x % mod + z % mod) % mod) * ((number[x] % mod + number[z] % mod) % mod)) % mod;
            ans += score % mod;
            ans %= mod;
        }
    }

    cout << ans % mod << endl;

    return 0;
}
