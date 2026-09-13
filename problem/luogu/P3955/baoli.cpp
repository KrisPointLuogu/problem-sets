#include <bits/stdc++.h>
using namespace std;

int n, q;

const int maxn = 1005;

int nums[maxn];
string s[maxn];

int main(int argc, char const *argv[])
{
    cin >> n >> q;
    for (int i = 1; i <= n; i++)
    {
        cin >> nums[i];
    }

    sort(nums + 1, nums + 1 + n);

    for (int i = 1; i <= n; i++)
    {
        s[i] = to_string(nums[i]);
    }

    while (q--)
    {
        int slen, posi;
        string substr;
        cin >> slen >> substr;
        for (int i = 1; i <= n; i++)
        {
            int pos = s[i].rfind(substr);
            if (pos != string::npos && pos == s[i].size() - substr.size())
            {
                // cout << s[i] << " " << pos << endl;
                posi = i;
                goto succeed;
            }
        }
        cout << -1 << endl;
        continue;
    succeed:
        cout << s[posi] << endl;
    }

    return 0;
}
