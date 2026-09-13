#include <bits/stdc++.h>
using namespace std;

/*
main.cpp:
我的思路：以每一个cube数为界，分割成几个队列，维护队列指针，依次输出
*/

const int maxn = 1e6 + 5;

int point[maxn];
int vis[maxn];
int pointvis[maxn];

int n;
vector<long long> a;
void init()
{
    cin >> n;
    a.resize(n + 5);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
}

int main(int argc, char const *argv[])
{
    int maxcube = 0;

    set<long long> cube; // i * i * i
    for (long long i = 1; i * i * i <= n; i++)
    {
        cube.insert(i * i * i);
        point[i] = i * i * i;
        maxcube = i;
    }

    vector<vector<int>> ans;
    while (1)
    {
        vector<int> arr;
        for (int i = 1; i <= maxcube; i++)
        {
            // CHULI POINT
            if (pointvis[i])
                break;
            int add = i;
            // CHULI POINT END

            // CHULI A vector
            while (!vis[point[i]])
                point[i]++;
            if (point[i] == point[i + 1])
                pointvis[i + 1] = 1;
            arr.push_back(a[point[i]]);
            // end

            // ADD POINT
            point[i] += add;
            if (point[i] > n)
            {
                pointvis[i] = 1;
                break;
            }
            // ADD POINT END
        }
        ans.push_back(arr);
    }

    int anssize = ans.size();
    cout << anssize << endl;
    for (int i = 1; i <= anssize; i++)
    {
        for (auto arr : ans)
        {
            for (auto arr2 : arr)
            {
                cout << arr2 << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
