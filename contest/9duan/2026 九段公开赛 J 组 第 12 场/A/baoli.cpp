#include <bits/stdc++.h>
using namespace std;

int n;
int c[1000005];

int main(int argc, char const *argv[])
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> c[i];
    }
    int ans = 0;
    do
    {
        for (int i = 1;; i += 3)
        {
            cout << i << " " << i + 1 << " " << i + 2 << endl;
            cout << c[i] << " " << c[i + 1] << " " << c[i + 2] << endl;
            cout << endl;
            if (c[i] == -1)
                break;
            if (c[i + 1] == -1)
            {
                ans += c[i];
                break;
            }
            if (c[i + 2] == -1)
            {
                ans += c[i] + c[i + 1];
                break;
            }
            ans += c[i] + c[i + 1];
        }

        cout << ans << endl;
    } while (next_permutation(c + 1, c + 1 + n));

    return 0;
}
