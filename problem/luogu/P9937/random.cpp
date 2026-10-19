#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int main() {
  int n = rnd(1, 5);
  int L = rnd(0, 5);
  cout << n << " " << L << "\n";
  for (int i = 0; i < n; i++) {
    cout << rnd(0, 10) << " \n"[i == n - 1];
  }
  return 0;
}
