// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T2《叠片检测》
// 子任务 3：30 分，测试点 06~11
// 对应题解章节：子任务 3：可行性动态规划
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    long long d, l;
    cin >> n >> m >> d >> l;
    vector<long long> s(n), need(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    // 本程序只负责子任务 3（N,M <= 2000）。它的可行性动态规划是 O(N^2) 的，
    // 规模再大就不是这一档该管的事，超出范围直接给一个非答案的标记退出。
    // 完整范围的做法见同目录的满分程序。
    if (n > 2000 || m > 2000) {
        cout << -13 << '\n';
        return 0;
    }
    sort(s.begin(), s.end());
    for (int j = 0; j < n; ++j) need[j] = l + (long long)(j / m) * d;
    vector<int> dp(n + 1, -1000000);
    dp[0] = 0;
    // dp[j] 记录前 j 个阈值槽位能否被填满。
    for (int i = 0; i < n; ++i) {
        for (int j = n - 1; j >= 0; --j) {
            if (dp[j] >= 0 && s[i] >= need[j]) dp[j + 1] = max(dp[j + 1], dp[j] + 1);
        }
    }
    int ans = 0;
    for (int j = 0; j <= n; ++j) if (dp[j] >= 0) ans = j;
    cout << ans << '\n';
    return 0;
}
