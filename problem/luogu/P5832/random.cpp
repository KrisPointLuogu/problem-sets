#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

char rnd_char() { return char('A' + rnd(0, 25)); }

int solve(const string &s, int n) {
  for (int k = 1; k <= n; ++k) {
    bool ok = true;
    for (int i = 0; i <= n - k; ++i) {
      string sub = s.substr(i, k);
      for (int j = i + 1; j <= n - k; ++j) {
        if (s.substr(j, k) == sub) {
          ok = false;
          goto end;
        }
      }
    }
  end:
    if (ok)
      return k;
  }
  return n;
}

int main() {
  int n = rnd(4, 10);
  string s;
  for (int i = 0; i < n; ++i)
    s += rnd_char();

  int ans = solve(s, n);

  cout << n << "\n";
  cout << s << "\n";
  return 0;
}
