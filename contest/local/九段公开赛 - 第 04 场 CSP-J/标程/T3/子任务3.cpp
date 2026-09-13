// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T3《监控触发》
// 子任务 3：15 分，测试点 06~08
// 对应题解章节：子任务 3：所有记录都有 i=j
// ==============================================================
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k, q;
    cin >> n >> m >> k >> q;
    vector<int> a(m + 1, 0);
    for (int i = 1; i <= m; i++) cin >> a[i];
    vector< vector<int> > days(n + 1);
    for (int d = 1; d <= k; d++) {
        int x;
        cin >> x;
        days[x].push_back(d);
    }

    // born[d] 统计第 d 次事件后首次满足 2*h_i>=a_t 的记录。
    // 关键转移：第 rank 次更新所在日期就是这条记录的首次触发日。
    vector<int> born(k + 2, 0);
    bool valid = true;
    for (int z = 0; z < q; z++) {
        int u, v, t;
        cin >> u >> v >> t;
        if (u != v) valid = false;
        if (u == v) {
            int rank = (a[t] + 1) / 2;
            if ((int)days[u].size() >= rank) born[days[u][rank - 1]]++;
        }
    }
    if (!valid) return 0;

    int answer = 0;
    for (int d = 1; d <= k; d++) {
        // 每次增加使 2*h_i 增加 2，第 rank 次增加正好给出首次触发日。
        answer += born[d];
        // 边界与答案：不存在所需第 rank 次增加的记录永远不进入累计值。
        cout << answer << '\n';
    }
    return 0;
}
