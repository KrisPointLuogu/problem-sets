#include <bits/stdc++.h>
using namespace std;

int n, s;

int main(int argc, char const *argv[])
{
    cin >> n >> s;
    int t = s;
    for (int i = 1; i <= n; i++)
    {
        string s;
        cin >> s;
        int pos1 = s.find("shuki");
        int pos2 = s.find("daishuki");
        int pos3 = s.find("kirai");
        if (pos3 != string::npos)
        {
            if (t >= 0)
                t = 0;
            continue;
        }
        if (pos2 != string::npos)
        {
            t += 2;
            continue;
        }
        if (pos1 != string::npos)
        {
            t++;
            continue;
        }

        if ((pos1 == string::npos) && (pos2 == string::npos) && (pos3 == string::npos))
        {
            t--;
        }
    }

    if (t > 0)
    {
        cout << t - s << endl;
    }
    else
    {
        cout << "shuki" << endl;
    }

    return 0;
}
