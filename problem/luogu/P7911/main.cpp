#include <bits/stdc++.h>
using namespace std;

map<string, long long> m;

bool check_ip(string s)
{
    vector<long long> tv;
    vector<char> sep;
    string ts;
    for (char c : s)
    {
        if (c == '.' || c == ':')
        {
            if (ts.empty())
                return false;
            if (ts.size() > 1 && ts[0] == '0')
                return false;
            long long x = 0;
            for (char d : ts)
            {
                if (d < '0' || d > '9')
                    return false;
                x = x * 10 + (d - '0');
                if (x > 1000000)
                    return false;
            }
            tv.push_back(x);
            sep.push_back(c);
            ts.clear();
        }
        else
        {
            ts += c;
        }
    }

    if (ts.empty())
        return false;
    if (ts.size() > 1 && ts[0] == '0')
        return false;
    long long x = 0;
    for (char d : ts)
    {
        if (d < '0' || d > '9')
            return false;
        x = x * 10 + (d - '0');
        if (x > 1000000)
            return false;
    }
    tv.push_back(x);

    if (tv.size() != 5)
        return false;
    if (sep.size() != 4)
        return false;
    if (sep[0] != '.' || sep[1] != '.' || sep[2] != '.' || sep[3] != ':')
        return false;
    for (long long i = 0; i < 4; i++)
    {
        if (tv[i] > 255)
            return false;
    }
    if (tv[4] > 65535)
        return false;

    return true;
}

int main()
{
    long long T;
    cin >> T;
    long long cnt = 0;
    while (T--)
    {
        cnt++;
        string class_str;
        string s;
        cin >> class_str >> s;
        if (!check_ip(s))
        {
            cout << "ERR" << endl;
            continue;
        }
        if (class_str == "Server")
        {
            if (m[s] == 0)
            {
                m[s] = cnt;
                cout << "OK" << endl;
                continue;
            }
            else if (m[s] != 0)
            {
                cout << "FAIL" << endl;
                continue;
            }
        }
        if (class_str == "Client")
        {
            if (m[s] != 0)
            {
                cout << m[s] << endl;
                continue;
            }
            else
            {
                cout << "FAIL" << endl;
                continue;
            }
        }
    }
    return 0;
}
