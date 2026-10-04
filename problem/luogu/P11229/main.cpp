#include <bits/stdc++.h>
using namespace std;

int T;
int n;

int tables[100]{
    -1, -1, 1, 7, 4, 2, 6, 8, 10, 18, 22, 20, 28, 68, 88, 108, 188, 200, 208, 288, 688, 888, 1088, 1888, 2008, 2088, 2888, 6888, 8888, 10888, 18888, 20088, 20888, 28888, 68888, 88888, 108888, 188888, 200888, 208888, 288888, 688888, 888888, 1088888, 1888888, 2008888, 2088888, 2888888, 6888888, 8888888, 10888888};

void print(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << '8';
    }
}

int main(int argc, char const *argv[])
{
    cin >> T;
    while (T--)
    {
        cin >> n;
        if (n <= 50)
        {
            cout << tables[n] << endl;
            continue;
        }
        if (n % 7 == 0)
        {
            print(n / 7);
        }
        if (n % 7 == 1)
        {
            cout << "10";
            print(n / 7 - 1);
        }
        if (n % 7 == 2)
        {
            cout << "1";
            print(n / 7);
        }
        if (n % 7 == 3)
        {
            cout << "200";
            print(n / 7 - 2);
        }
        if (n % 7 == 4)
        {
            cout << "20";
            print(n / 7 - 1);
        }
        if (n % 7 == 5)
        {
            cout << "2";
            print(n / 7);
        }
        if (n % 7 == 6)
        {
            cout << '6';
            print(n / 7);
        }
        cout << endl;
    }
    return 0;
}
