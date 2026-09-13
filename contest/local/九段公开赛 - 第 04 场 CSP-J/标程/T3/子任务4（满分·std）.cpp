// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T3《监控触发》
// 子任务 4：60 分，测试点 09~20
// 对应题解章节：满分做法：合并两张表求第 need 小
// 本程序即标准解（std），可通过全部测试点。
// ==============================================================
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k, q;
    cin >> n >> m >> k >> q;
    vector<int> a(m + 1, 0);
    for (int i = 1; i <= m; i++) cin >> a[i];

    // days[x] 按升序保存计数器 x 被增加的事件编号。
    vector< vector<int> > days(n + 1);
    for (int d = 1; d <= k; d++) {
        int x;
        cin >> x;
        days[x].push_back(d);
    }

    // born[d] 表示恰好在第 d 次事件后首次触发的记录数。
    vector<int> born(k + 2, 0);
    for (int z = 0; z < q; z++) {
        int u, v, t;
        cin >> u >> v >> t;
        int need = a[t];
        int day = k + 1;

        if (u == v) {
            int rank = (need + 1) / 2;
            if ((int)days[u].size() >= rank) day = days[u][rank - 1];
        } else if ((int)days[u].size() + (int)days[v].size() >= need) {
            // 不变量：切分左右两表的前 need 个日期，左侧值都不大于右侧值。
            int lo = max(0, need - (int)days[v].size());
            int hi = min(need, (int)days[u].size());
            while (lo <= hi) {
                int x = (lo + hi) / 2;
                int y = need - x;
                int left_u = (x == 0 ? 0 : days[u][x - 1]);
                int right_u = (x == (int)days[u].size() ? k + 1 : days[u][x]);
                int left_v = (y == 0 ? 0 : days[v][y - 1]);
                int right_v = (y == (int)days[v].size() ? k + 1 : days[v][y]);
                if (left_u <= right_v && left_v <= right_u) {
                    day = max(left_u, left_v);
                    break;
                }
                if (left_u > right_v) hi = x - 1;
                else lo = x + 1;
            }
        }
        if (day <= k) born[day]++;
    }

    // 边界与答案：先加入当天新触发记录，再输出当前累计触发总数。
    int answer = 0;
    for (int d = 1; d <= k; d++) {
        answer += born[d];
        cout << answer << '\n';
    }
    return 0;
}
