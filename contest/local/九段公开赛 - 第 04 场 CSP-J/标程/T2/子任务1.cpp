// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T2《叠片检测》
// 子任务 1：10 分，测试点 01~02
// 对应题解章节：子任务 1：枚举选哪些样片
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    long long d, l;
    cin >> n >> m >> d >> l;
    vector<long long> s(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    if (n > 5 || m > 5) {
        cout << -7 << '\n';
        return 0;
    }
    int best = 0;
    // 每个选出的子集，都拿去和同样数量的最浅槽位做匹配。
    for (int mask = 0; mask < (1 << n); ++mask) {
        vector<long long> chosen;
        for (int i = 0; i < n; ++i) if (mask & (1 << i)) chosen.push_back(s[i]);
        sort(chosen.begin(), chosen.end());
        bool ok = true;
        for (int i = 0; i < (int)chosen.size(); ++i) {
            long long need = l + (long long)(i / m) * d;
            if (chosen[i] < need) ok = false;
        }
        if (ok) best = max(best, (int)chosen.size());
    }
    cout << best << '\n';
    return 0;
}
