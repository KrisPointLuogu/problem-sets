#include <bits/stdc++.h>
using namespace std;

random_device __rnd;

default_random_engine rnd(__rnd());

long long get_rnd(long long l, long long r) { return rnd() % (r - l + 1) + l; }

int main(int argc, char const *argv[])
{
  cout << get_rnd(-100, 100) << " " << get_rnd(-100, 100) << endl;
  return 0;
}
