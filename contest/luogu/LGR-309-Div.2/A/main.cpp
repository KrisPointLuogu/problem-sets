#include <bits/stdc++.h>
using namespace std;

int T;

int main(int argc, char const *argv[])
{
    cin >> T;
    cout << "x     a%b     b%a      gcd(a,b)" << endl;
    while (T--)
    {
        int a, b;
        cin >> a >> b;
        for (int x = a + b + 1;; x++)
        {
            if (x % b == a % b && x % a == b % a)
            {
                cout << x << "   " << a % b << "   " << b % a << "   " << gcd(a, b);
                cout << endl;
                break;
            }
        }
    }
    return 0;
}
