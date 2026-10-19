#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int main(int argc, char *argv[])
{
  int T = 10;
  int maxN = 1e5;
  if (argc > 1)
    T = atoi(argv[1]);
  if (argc > 2)
    maxN = atoi(argv[2]);

  cout << T << "\n";
  set<int> used;
  for (int i = 0; i < T; i++)
  {
    int n;
    do
    {
      n = rnd(1, maxN);
    } while (used.count(n));
    used.insert(n);
    cout << n << "\n";
  }
  return 0;
}
