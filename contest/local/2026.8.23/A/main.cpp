#include <bits/stdc++.h>
using namespace std;

string n;
int k;
int b[105];

int chai(string s)
{
    reverse(s.begin(), s.end());
    int cnt = 0;
    for (auto c : s)
    {
        b[++cnt] = c - '0';
    }
    return cnt;
}

bool check(int b[], int n)
{
    for (int i = 1; i <= n; i++)
    {
        if (b[i] >= 10)
            return 0;
    }
    return 1;
}

int add(int b[], int n)
{
    bool flag = 0;
    for (int i = 1; i <= n; i++)
    {
        if (b[i] >= 10)
        {
            b[i] = 0;
            b[i + 1]++;
            if (i == n)
                flag = 1;
        }
    }
    if (flag)
        n++;
    return n;
}

void print(int b[], int n, int end = 1)
{
    reverse(b + 1, b + 1 + n);
    for (int i = 1; i <= n; i++)
    {
        cout << b[i];
    }
    if (end)
        cout << " -> ";
    reverse(b + 1, b + 1 + n);
}

void work()
{
    int len = chai(n);
    for (int i = 1; i <= k; i++)
    {
        print(b, len, !(i == k));
        // 第 i 位
        if (b[i] <= 4)
        {
            for (int j = 1; j <= i; j++)
            {
                b[i] = 0;
            }
        }
        if (b[i] > 4)
        {
            for (int j = 1; j <= i; j++)
            {
                b[i] = 0;
            }
            b[i + 1]++;
        }
        int flag = check(b, len);
        if (!flag)
            len = add(b, len);

        if (i == k)
        {
            cout << endl;
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    int t;
    cin >> t;
    while (t--)
    {
        cin >> k >> n;
        work();
    }

    return 0;
}
