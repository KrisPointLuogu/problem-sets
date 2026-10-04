#include <bits/stdc++.h>
using namespace std;

int c, l;

struct cow
{
    int mxs; // maxspf
    int mns; // minspf
} cows[100005];

struct spf
{
    int spf;
    int cover;
} spfs[100005];

int main(int argc, char *argv[])
{
    cin >> c >> l;
    for (int i = 1; i <= c; i++)
    {
        cin >> cows[i].mns >> cows[i].mxs;
    }
    for (int i = 1; i <= l; i++)
    {
        cin >> spfs[i].spf >> spfs[i].cover;
    }

    sort(cows + 1, cows + 1 + c, [](cow &a, cow &b)
         { return a.mxs < b.mxs; });

    sort(spfs + 1, spfs + 1 + l, [](spf &a, spf &b)
         { return a.spf < b.spf; });

    int ans = 0;

    for (int i = 1; i <= c; i++)
    {
        for (int j = 1; j <= l; j++)
        {
            if (spfs[j].cover <= 0)
                continue;
            if (cows[i].mns <= spfs[j].spf && spfs[j].spf <= cows[i].mxs)
            {
                ans++;
                spfs[j].cover--;
                break;
            }
        }
    }

    cout << ans << "\n";

    return 0;
}
