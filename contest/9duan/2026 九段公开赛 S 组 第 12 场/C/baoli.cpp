#include <bits/stdc++.h>
using namespace std;

int q;
vector<pair<int, bool>> v;

int main(int argc, char const *argv[])
{
    cin >> q;
    while (q--)
    {
        int t, p, l;
        cin >> t >> p;
        if (t == 3)
            cin >> l;

        // cout << "---START---" << endl;
        // for (auto c : v)
        // {
        //     cout << c.first << " " << c.second << endl;
        // }
        // cout << "---END---" << endl;

        if (t == 1)
        {
            v.push_back({p, 1});
        }
        else if (t == 2)
        {
            for (auto &c : v)
            {
                if (c.first == p && c.second == 1)
                {
                    c.second = 0;
                    // cout << "---START2---" << endl;
                    // cout << c.first << " " << c.second << endl;
                    // cout << "---END---" << endl;
                    break;
                }
            }
        }
        else if (t == 3)
        {
            int cnt = 0;
            for (auto c : v)
            {
                if (c.second == 0)
                    continue;
                int t = p;
                t ^= c.first;
                if (t < l)
                {
                    // cout << "---START3---" << endl;
                    // cout << c.first << " " << c.second << endl;
                    // cout << "---END---" << endl;
                    cnt++;
                }
            }
            cout << cnt << endl;
        }
    }
    return 0;
}
