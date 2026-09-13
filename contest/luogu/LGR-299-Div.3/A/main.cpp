#include <bits/stdc++.h>
using namespace std;

const int max_y = 5e8;

int main(int argc, char const *argv[])
{
    long long n;
    cin >> n;
    if (n > max_y)
    {
        cout << "O(1)" << endl;
    }
    else
    {
        long long sum = n * n;
        if (sum <= max_y)
        {
            cout << "O(n^2)" << endl;
        }
        else
        {
            cout << "O(n)" << endl;
        }
    }
    return 0;
}
