#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

// 生成 [l, r] 之间的随机整数。
int rnd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

int main() {
    int n = rnd(1, 7);
    cout << n << "\n";
    for (int i = 1; i <= n; i++) {
        cout << rnd(1, 3) << (i == n ? "\n" : " ");
    }
    return 0;
}
