#include <bits/stdc++.h>
using namespace std;

/*
main.cpp:
我的思路：以每一个cube数为界，分割成几个队列，维护队列指针，依次输出
*/

int main(int argc, char const *argv[])
{
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    set<long long> cube; // i * i * i
    for (long long i = 1; i * i * i <= n; i++)
    {
        cube.insert(i * i * i);
    }

    vector<vector<long long>> ans;
    while (a.size())
    {
        vector<long long> row;  // 被del
        vector<long long> keep; // keep proceed.
        for (int i = 0; i < (int)a.size(); i++)
        {
            if (cube.count((long long)i + 1))
            {
                row.push_back(a[i]);
            }
            else
            {
                keep.push_back(a[i]);
            }
        }
        ans.push_back(row);
        a = keep;
    }

    cout << ans.size() << endl;
    for (auto row : ans)
    {
        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j] << " ";
        }
        cout << endl;
    }

    return 0;
}
