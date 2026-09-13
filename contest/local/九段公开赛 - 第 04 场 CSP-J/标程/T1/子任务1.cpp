// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T1《档案核销》
// 子任务 1：20 分，测试点 01~04
// 对应题解章节：子任务 1：照定义逐轮核销
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    string s;
    cin >> n >> k >> s;
    if (n > 2000) {
        cout << "invalid-small-tier\n";
        return 0;
    }
    vector<int> removed(n, 0);
    // 每一轮都精确地找出最小标签，以及它最靠左的那个位置。
    for (int step = 0; step < k; ++step) {
        int best = -1;
        for (int i = 0; i < n; ++i) {
            if (!removed[i] && (best == -1 || s[i] < s[best])) best = i;
        }
        removed[best] = 1;
    }
    // 未被删除的位置保持原来的先后顺序。
    string answer;
    for (int i = 0; i < n; ++i) if (!removed[i]) answer += s[i];
    if (answer.empty()) cout << "EMPTY\n";
    else cout << answer << '\n';
    return 0;
}
