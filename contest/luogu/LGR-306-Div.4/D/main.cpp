#include <bits/stdc++.h>
using namespace std;

string s;

bool is_prime(long long sum)
{
    if (sum < 2)
        return 0;
    for (int i = 2; 1LL * i * i <= sum; i++)
    {
        if (sum % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    cin >> s;
    long long ans = 0;
    for (int i = 0; i < s.size() - 1; i++)
    {
        long long sum = (s[i] - '0') * 10 + (s[i + 1] - '0');
        if (is_prime(sum))
            ans += sum;
    }

    cout << ans << endl;

    return 0;
}
