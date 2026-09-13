// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T4《数据抢救》
// 子任务 2：15 分，测试点 04~06
// 对应题解章节：子任务 2：只有一个数据卷
// ==============================================================
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, T;
    cin >> n >> T;
    int cnt, base_time, base_value;
    cin >> cnt >> base_time >> base_value;
    vector<pair<int, int> > items(cnt);
    for (int j = 0; j < cnt; ++j) cin >> items[j].first >> items[j].second;
    for (int i = 1; i < n; ++i) {
        int extra;
        cin >> extra >> base_time >> base_value;
        for (int j = 0, t, c; j < extra; ++j) cin >> t >> c;
    }
    if (n != 1) {
        cout << 0 << '\n';
        return 0;
    }

    // 状态含义：dp[w] 是付掉索引块之后，容量 w 以内数据块的最大价值。
    if (base_time > T) {
        cout << 0 << '\n';
        return 0;
    }
    vector<long long> dp(T - base_time + 1, 0);
    // 转移不变量：倒序更新保证每个数据块最多用一次。
    for (int j = 0; j < cnt; ++j) {
        int t = items[j].first;
        int c = items[j].second;
        for (int w = T - base_time; w >= t; --w) {
            dp[w] = max(dp[w], dp[w - t] + c);
        }
    }
    // 边界与答案：什么都不选得 0；否则要把索引块的价值补上一次。
    cout << max(0LL, dp[T - base_time] + base_value) << '\n';
    return 0;
}
