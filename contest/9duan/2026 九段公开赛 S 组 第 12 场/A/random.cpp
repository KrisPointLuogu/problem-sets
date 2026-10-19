#include <bits/stdc++.h>
using namespace std;

random_device __rnd;
default_random_engine rd(__rnd());

int get_lr(int l, int r)
{
    return rd() % (r - l + 1) + l;
}

int main(int argc, char const *argv[])
{
    cout << 1 << endl;
    int n = get_lr(3, 6);
    int S = get_lr(3, 10);
    cout << n << " " << S << endl;
    int t = get_lr(2, 20);
    for (int i = 1; i <= n; i++)
        cout << get_lr(2, 20) << " ";
    cout << endl;

    for (int i = 1; i <= n; i++)
        cout << t << " ";
    cout << endl;

    return 0;
}
