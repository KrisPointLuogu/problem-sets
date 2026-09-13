#include <bits/stdc++.h>
#define maxn 2333
using namespace std;

char buf[maxn];
int n;
vector<int> a, b;

int main()
{
    scanf("%d%s", &n, buf);
    for (int i = 0, len = strlen(buf); i < len; i++)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
            a.push_back(buf[i] - '0');
        else
            a.push_back(islower(buf[i]) ? buf[i] - 'a' + 10 : buf[i] - 'A' + 10);
    }
    b = a;
    reverse(a.begin(), a.end());
    if (a == b)
        return puts("STEP=0"), 0;
    for (int ans = 1; ans <= 30; ans++)
    {
        for (int i = 0, siz = a.size(); i < siz; i++)
        {
            a[i] += b[i]; // 加法
            if (i != siz - 1)
                a[i + 1] += a[i] / n; // 处理进位
            else if (a[i] >= n)
                a.push_back(a[i] / n);
            a[i] %= n;
        }
        b = a;
        reverse(b.begin(), b.end());
        if (a == b)
        {
            printf("STEP=%d", ans);
            return 0;
        }
    }
    puts("Impossible!");
    return 0;
}
