// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T1《档案核销》
// 子任务 3：15 分，测试点 07~09
// 对应题解章节：子任务 3：标签非降
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    string s;
    cin >> n >> k >> s;
    bool sorted = true;
    for (int i = 1; i < n; ++i) if (s[i] < s[i - 1]) sorted = false;
    // 标签非降时，删除优先级恰好就是下标顺序。
    if (sorted) {
        string answer = s.substr(k);
        if (answer.empty()) cout << "EMPTY\n";
        else cout << answer << '\n';
    } else cout << "invalid-sorted-tier\n";
    return 0;
}
