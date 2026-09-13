#include <bits/stdc++.h>
using namespace std;

int n;
string s;

int main(int argc, char const *argv[])
{
    cin >> n >> s;

    int cnt = 0;

    for (int i = 0; i < n; i++)
    {
        if (s[i] == '1')
            continue;
        if (s[i] == '0')
        {
            int t = i + 1;
            for (int j = i + 1; j <= n; j += t)
            {
                if (s[j - 1] == '1')
                    s[j - 1] = '0';
                else
                    s[j - 1] = '1';
            }

            cnt++;
        }
    }

    cout << cnt << endl;

    return 0;
}
