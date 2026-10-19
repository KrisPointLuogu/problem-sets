#include <bits/stdc++.h>
using namespace std;

const int maxn = 1e5 + 5;
const int maxs = 1e6 + 5;

string input_s;
string outpre_s;
int n;
int a[maxn];

void init()
{
    getline(cin, input_s);
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
}

void work()
{
    stack<int> sta;
    for (int i = 0; i < (int)outpre_s.size(); i++)
    {
        char c = outpre_s[i];
        if (c == ' ')
            continue;

        if (c == 'x')
        {
            int num = 0;
            while (i + 1 < (int)outpre_s.size() && isdigit(outpre_s[i + 1]))
            {
                i++;
                num = num * 10 + (outpre_s[i] - '0');
            }
            sta.push(a[num]);
        }

        if (c == '&')
        {
            int right = sta.top();
            sta.pop();
            int left = sta.top();
            sta.pop();
            sta.push(left & right);
        }
        if (c == '|')
        {
            int right = sta.top();
            sta.pop();
            int left = sta.top();
            sta.pop();
            sta.push(left | right);
        }
        if (c == '!')
        {
            int top = sta.top();
            sta.pop();
            sta.push(!top);
        }
    }
    cout << sta.top() << endl;
}

int main(int argc, char const *argv[])
{
    init();
    outpre_s = input_s;
    int q;
    cin >> q;
    while (q--)
    {
        int idx;
        cin >> idx;
        a[idx] ^= 1;
        work();
        a[idx] ^= 1;
    }
    return 0;
}
