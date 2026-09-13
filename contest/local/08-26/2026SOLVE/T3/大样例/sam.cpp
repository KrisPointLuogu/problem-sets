#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
int sum[N][55];
int main() {
	freopen("ex.in" , "r" , stdin);
	freopen("1.out" , "w" , stdout);
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
    int n;
    cin >> n;
    
    for (int i = 1; i <= n; ++i) {
        int d;
        cin >> d;
        for (int j = 1; j <= 50; ++j) {
            sum[i][j] = sum[i-1][j];
        }
        sum[i][d]++;
    }
    int m;
    cin >> m;
    while (m--) {
        int l, r;
        cin >> l >> r;
        int tot = 0;
        int p = 0;
        for (int j = 1; j <= 50; ++j) {
            int cnt = sum[r][j] - sum[l-1][j];
            if (cnt > 0) {
                if (p == 0 || j - p <= 1) {
                    tot += cnt;
                    p = j;
                } else {
                    break;
                }
            }
        }
        cout << tot << '\n';
    }
    return 0;
}

