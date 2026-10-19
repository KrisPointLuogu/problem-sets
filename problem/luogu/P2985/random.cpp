#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int main() {
  int n = rnd(1, 5);
  int d = rnd(1, 4);
  cout << n << " " << d << "\n";
  for (int i = 0; i < n; i++) {
    cout << rnd(2, 10) << "\n";
  }
  return 0;
}
