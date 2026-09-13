// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T3《监控触发》
// 子任务 1：15 分，测试点 01~03
// 对应题解章节：子任务 1：照定义逐次事件重算
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
    vector<int> u(q), v(q), t(q);
    for (int z = 0; z < q; z++) cin >> u[z] >> v[z] >> t[z];

    // h[x] 是当前事件结束后计数器 x 的值，初始全部为 0。
    vector<int> h(n + 1, 0);
    // 这一小规模做法直接扫描，不需要完整范围中的二分查找与前缀和。
    for (int d = 1; d <= k; d++) {
        h[event[d]]++;
        int answer = 0;
        // 每轮逐条执行定义中的判断，重复记录也各计一次。
        for (int z = 0; z < q; z++) {
            if (h[u[z]] + h[v[z]] >= a[t[z]]) answer++;
        }
        // 边界与答案：当天更新必须先发生，随后才输出本轮计数。
        cout << answer << '\n';
    }
    return 0;
}
