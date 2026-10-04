#include <bits/stdc++.h>
using namespace std;

int T;

int b[50];

int stick[10]{
    6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int num_3x(int n)
{
    memset(b, 0, sizeof(b));
    int len = 0;
    while (n != 0)
    {
        b[++len] = n % 10;
        n /= 10;
    }
    return len;
}

int work(int n)
{
    for (int i = 1; i <= 88888888; i++)
    {
        int len = num_3x(i);
        int num = 0;
        for (int i = 1; i <= len; i++)
        {
            num += stick[b[i]];
            if (num > n)
            {
                break;
            }
        }
        if (num == n)
        {
            // cout << i << endl;
            return i;
            // return;
        }
    }
    // cout << "-1" << endl;
    return -1;
}

int main(int argc, char const *argv[])
{
    int n;
    cin >> T;
    while (T--)
    {
        cin >> n;
        work(n);
    }

    return 0;
}
