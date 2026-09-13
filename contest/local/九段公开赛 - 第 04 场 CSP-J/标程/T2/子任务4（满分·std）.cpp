// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T2《叠片检测》
// 子任务 4：45 分，测试点 12~20
// 对应题解章节：满分做法：升序扫描的贪心
// 本程序即标准解（std），可通过全部测试点。
// ==============================================================
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    long long d, l;
    cin >> n >> m >> d >> l;
    vector<long long> s(n);
    for (int i = 0; i < n; ++i) cin >> s[i];
    sort(s.begin(), s.end());

    // 已分配的 ans 片样片，占的正是所有检测匣里最浅的 ans 个槽位。
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        long long need = l + (long long)(ans / m) * d;
        if (s[i] >= need) ans++;
    }
    cout << ans << '\n';
    return 0;
}
