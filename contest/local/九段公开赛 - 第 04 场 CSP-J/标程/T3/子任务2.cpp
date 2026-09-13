// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T3《监控触发》
// 子任务 2：10 分，测试点 04~05
// 对应题解章节：子任务 2：只有一条记录
// ==============================================================
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k, q;
    cin >> n >> m >> k >> q;
    vector<int> a(m + 1, 0), event(k + 1, 0);
    for (int i = 1; i <= m; i++) cin >> a[i];
    for (int d = 1; d <= k; d++) cin >> event[d];
    if (q != 1) return 0;
    int u, v, t;
    cin >> u >> v >> t;

    // h 只维护唯一记录会用到的两个端点，但同端点仍自然计算两次。
    vector<int> h(n + 1, 0);
    for (int d = 1; d <= k; d++) {
        h[event[d]]++;
        // 不变量：唯一记录的状态由当前 h[u]+h[v] 完全决定。
        int answer = (h[u] + h[v] >= a[t] ? 1 : 0);
        // 边界与答案：阈值在本次增加后达到时，当天就输出 1。
        cout << answer << '\n';
    }
    return 0;
}
