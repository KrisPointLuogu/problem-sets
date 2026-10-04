#include <bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[])
{
    set<string> s;
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        string str;
        cin >> str;
        s.insert(str);
    }
    cout << 52 - s.size() << endl;
    return 0;
}

