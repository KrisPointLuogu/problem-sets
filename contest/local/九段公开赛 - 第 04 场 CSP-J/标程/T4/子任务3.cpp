// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T4《数据抢救》
// 子任务 3：15 分，测试点 07~09
// 对应题解章节：子任务 3：所有卷都没有数据块
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
    vector<pair<int, int> > bases;
    bool base_only = true;
    for (int i = 0; i < n; ++i) {
        int cnt, t, c;
        cin >> cnt >> t >> c;
        bases.push_back(make_pair(t, c));
        if (cnt != 0) base_only = false;
        for (int j = 0, x, y; j < cnt; ++j) cin >> x >> y;
    }
    if (!base_only) {
        cout << 0 << '\n';
        return 0;
    }

    // 状态含义：dp[w] 是容量 w 以内，只取索引块能得到的最大价值。
    vector<long long> dp(T + 1, 0);
    // 转移不变量：倒序更新保证每个索引块只被取一次（01 背包）。
    for (int i = 0; i < n; ++i) {
        int t = bases[i].first;
        int c = bases[i].second;
        for (int w = T; w >= t; --w) {
            dp[w] = max(dp[w], dp[w - t] + c);
        }
    }
    // 边界与答案：允许容量用不满，所以 dp[T] 就是最终最大值。
    cout << dp[T] << '\n';
    return 0;
}
