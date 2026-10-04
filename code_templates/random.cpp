#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

// 生成 [l, r] 之间的随机整数。
int rnd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

// 生成 [l, r] 之间的随机实数。
double rnd(double l, double r) {
    return uniform_real_distribution<double>(l, r)(rng);
}

// 以概率 p 返回 true。
bool hit(double p) {
    return uniform_real_distribution<double>(0.0, 1.0)(rng) <= p;
}

// 随机打乱序列。
template<typename T>
void shuffle(vector<T>& a) {
    std::shuffle(a.begin(), a.end(), rng);
}

int main() {
    int n = rnd(4, 7);
    cout << n << "\n";
    return 0;
}
