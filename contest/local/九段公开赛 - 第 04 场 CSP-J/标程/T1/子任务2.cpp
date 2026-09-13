// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T1《档案核销》
// 子任务 2：10 分，测试点 05~06
// 对应题解章节：子任务 2：全部核销
// ==============================================================
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    string s;
    cin >> n >> k >> s;
    // 全部核销时，答案就是一个空序列。
    if (k == n) cout << "EMPTY\n";
    else cout << "invalid-erase-all-tier\n";
    return 0;
}
