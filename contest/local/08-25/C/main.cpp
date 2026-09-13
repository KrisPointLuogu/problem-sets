#include <bits/stdc++.h>
using namespace std;
vector<pair<char, long long>> seg;
int main()
{
    string s;
    long long c;
    cin >> s >> c;

    char nowchar = 0;
    long long charnum = 0;

    for (char ch : s)
    {
        if (isalpha(ch))
        {
            if (charnum != 0)
                seg.push_back({nowchar, charnum});
            nowchar = ch;
            charnum = 0;
        }
        else if (isdigit(ch))
        {
            charnum = charnum * 10 + (ch - '0');
        }
    }
    if (charnum != 0)
        seg.push_back({nowchar, charnum});

    long long len = 0;
    for (auto p : seg)
        len += p.second;

    long long div = c % len;
    long long pos = 0;
    for (auto p : seg)
    {
        if (pos + p.second > div)
        {
            cout << p.first << endl;
            return 0;
        }
        pos += p.second;
    }

    return 0;
}