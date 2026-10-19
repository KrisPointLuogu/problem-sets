#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

int main() {
    int n = rnd(1, 10);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        cout << rnd(1, 20) << " \n"[i == n - 1];
    }
    int L = rnd(1, 30);
    int R = rnd(L, 30);
    cout << L << " " << R << "\n";
    return 0;
}
