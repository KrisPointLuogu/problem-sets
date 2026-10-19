
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
map<string, int> state;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll n;
  cin >> n;
  for (ll i = 1; i <= n; i++) {
    string name;
    cin >> name;
    state[name] = 1;
  }

  ll m;
  cin >> m;
  for (ll i = 1; i <= m; i++) {
    string name;
    cin >> name;

    auto it = state.find(name);
    if (it == state.end()) {
      cout << "WRONG\n";
    } else if (it->second == 1) {
      it->second = 2;
      cout << "OK\n";
    } else {
      cout << "REPEAT\n";
    }
  }

  return 0;
}
