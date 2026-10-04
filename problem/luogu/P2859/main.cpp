#include <bits/stdc++.h>
using namespace std;

const int maxn = 50005;

int n;

struct cow
{
    int s, t;
    int id;
    int startid;

    int operator<(const cow &a) const
    {
        return t < a.t;
    }

} cows[maxn];

vector<priority_queue<cow>> vpq;
vector<int> ans_id(maxn);

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> cows[i].s >> cows[i].t;
        cows[i].startid = i;
    }

    sort(cows + 1, cows + 1 + n, [](cow &a, cow &b)
         { return a.s < b.s; });

    priority_queue<cow> pq;
    pq.push(cows[1]);
    vpq.push_back(pq);
    int ans = 1;
    // cows[1].id = 1;
    ans_id[cows[1].startid] = 1;
    for (int i = 2; i <= n; i++)
    {
        bool flag = 1;
        for (int j = 0; j < vpq.size(); j++)
        // for (auto &cowpq : vpq)
        {
            priority_queue<cow> &cowpq = vpq[j];
            if (cowpq.top().t < cows[i].s)
            {
                flag = 0;
                cowpq.pop();
                cowpq.push(cows[i]);
                // cows[i].id = (j + 1);
                ans_id[cows[i].startid] = j + 1;
                break;
            }
        }
        if (flag)
        {
            priority_queue<cow> pq;
            pq.push(cows[i]);
            vpq.push_back(pq);
            // cows[i].id = vpq.size();
            ans_id[cows[i].startid] = vpq.size();
            ans++;
        }
    }

    cout << ans << endl;
    for (int i = 1; i <= n; i++)
    {
        cout << ans_id[i] << endl;
    }

    return 0;
}
