#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e6 + 5;

int T;

int n;
string s;

// int b[maxn];

// void preinit()
// {
//     b[0] = 0;
//     for (int i = 1; i <= maxn - 5; i++)
//     {
//         if ((i % 3 == 1) || (i % 3 == 0))
//         {
//             b[i] = b[i - 1] + 1;
//         }
//         if (i % 3 == 2)
//         {
//             b[i] = b[i - 1];
//         }
//     }
//     // for (int i = 1; i <= 10; i++)
//     // {
//     //     cout << b[i] << " ";
//     // }
//     // cout << endl;
// }

// map<char, int> charmap;

void init()
{
    cin >> n >> s;
    // for (int i = 0; i < n; i++)
    // {
    //     charmap[s[i]]++;
    // }
}

void work()
{
    string t = "", cur = "";
    int ans = 0;
    for (char c : s)
    {
        cur += c;
        if (cur != t)
        {
            swap(t, cur);
            cur = "";
            ans++;
        }
    }
    cout << ans << endl;
}

int main(int argc, char const *argv[])
{
    // preinit();
    cin >> T;
    while (T--)
    {
        init();
        work();
    }
    return 0;
}
