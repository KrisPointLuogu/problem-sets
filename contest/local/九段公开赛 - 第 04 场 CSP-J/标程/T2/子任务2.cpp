// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T2《叠片检测》
// 子任务 2：15 分，测试点 03~05
// 对应题解章节：子任务 2：没有读数损耗
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    long long d, l, x;
    cin >> n >> m >> d >> l;
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        if (x >= l) ans++;
    }
    // 损耗为零时，每个位置的阈值都是同一个 L。
    if (d == 0) cout << ans << '\n';
    else cout << -11 << '\n';
    return 0;
}
