#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

int main() {
    int n = rnd(1, 10);
    int m = rnd(1, 10);
    int total = n * m;

    vector<int> pool(100);
    iota(pool.begin(), pool.end(), 1);
    shuffle(pool.begin(), pool.end(), rng);

    vector<int> a(pool.begin(), pool.begin() + total);
    shuffle(a.begin(), a.end(), rng);

    cout << n << ' ' << m << '\n';
    for (int i = 0; i < total; ++i) {
        cout << a[i] << " \n"[i + 1 == total];
    }
    return 0;
}
