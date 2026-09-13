// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T4《数据抢救》
// 子任务 1：15 分，测试点 01~03
// 对应题解章节：子任务 1：枚举所有块的子集
// ==============================================================
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, T;
    cin >> n >> T;
    vector<int> cost, value, need;
    bool in_range = n <= 5 && T <= 30;
    for (int i = 0; i < n; ++i) {
        int cnt, t, c;
        cin >> cnt >> t >> c;
        in_range = in_range && cnt <= 3;
        int base = (int)cost.size();
        cost.push_back(t);
        value.push_back(c);
        need.push_back(-1);
        for (int j = 0; j < cnt; ++j) {
            cin >> t >> c;
            cost.push_back(t);
            value.push_back(c);
            need.push_back(base);
        }
    }
    if (!in_range || cost.size() > 20) {
        cout << 0 << '\n';
        return 0;
    }

    // 状态含义：每个 mask 就是一整套选择方案，need[x] 记录它依赖的索引块。
    long long ans = 0;
    unsigned long long total = 1ULL << cost.size();
    for (unsigned long long mask = 0; mask < total; ++mask) {
        int used = 0;
        long long sum = 0;
        bool ok = true;
        for (int j = 0; j < (int)cost.size(); ++j) {
            if ((mask >> j) & 1ULL) {
                used += cost[j];
                sum += value[j];
                if (need[j] >= 0 && ((mask >> need[j]) & 1ULL) == 0) ok = false;
            }
        }
        // 转移不变量：只有容量够、且前置索引块全都选了，才允许更新。
        if (ok && used <= T) ans = max(ans, sum);
    }
    // 边界与答案：允许空集，ans 保存最终最大值。
    cout << ans << '\n';
    return 0;
}
