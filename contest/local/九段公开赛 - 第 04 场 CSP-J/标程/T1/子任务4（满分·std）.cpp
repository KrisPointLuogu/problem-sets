// ==============================================================
// 2026 九段公开赛　第 04 场　CSP-J　T1《档案核销》
// 子任务 4：55 分，测试点 10~20
// 对应题解章节：满分做法：按字母分配核销名额
// 本程序即标准解（std），可通过全部测试点。
// ==============================================================
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, k;
    string s;
    cin >> n >> k >> s;

    // cnt[c] 表示标签 c 还要核销掉最靠前的几份。
    int cnt[26] = {0};
    for (int i = 0; i < n; ++i) cnt[s[i] - 'a']++;
    for (int c = 0; c < 26; ++c) {
        int take = min(k, cnt[c]);
        cnt[c] = take;
        k -= take;
    }

    // 从左往右扫描，删掉的自然就是每种标签最靠前的那几次出现。
    string answer;
    for (int i = 0; i < n; ++i) {
        int c = s[i] - 'a';
        if (cnt[c] > 0) cnt[c]--;
        else answer += s[i];
    }
    if (answer.empty()) cout << "EMPTY\n";
    else cout << answer << '\n';
    return 0;
}
