// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T4《数据抢救》
// 子任务 4：55 分，测试点 10~20
// 对应题解章节：满分做法：按卷分层的依赖背包
// 本程序即标准解（std），可通过全部测试点。
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
    vector<long long> dp(T + 1, 0);
    const long long bad = -(1LL << 60);

    for (int i = 0; i < n; ++i) {
        int cnt, base_time, base_value;
        cin >> cnt >> base_time >> base_value;
        vector<pair<int, int> > items(cnt);
        for (int j = 0; j < cnt; ++j) {
            cin >> items[j].first >> items[j].second;
        }

        // 状态含义：take[w] 表示已经选了本卷索引块之后，容量 w 对应的最大价值。
        vector<long long> take(T + 1, bad);
        for (int w = base_time; w <= T; ++w) {
            take[w] = dp[w - base_time] + base_value;
        }
        // 转移不变量：倒序更新保证每个数据块只是一次 01 选择。
        for (int j = 0; j < cnt; ++j) {
            int cost = items[j].first;
            int value = items[j].second;
            for (int w = T; w >= cost; --w) {
                if (take[w - cost] != bad) {
                    take[w] = max(take[w], take[w - cost] + value);
                }
            }
        }
        // 边界与答案：保留 dp 表示跳过本卷，take 则已经付过本卷索引块的代价。
        for (int w = 0; w <= T; ++w) {
            dp[w] = max(dp[w], take[w]);
        }
    }

    cout << dp[T] << '\n';
    return 0;
}
