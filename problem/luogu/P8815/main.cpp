#include <bits/stdc++.h>
using namespace std;

string s;
string poland_s;

int get_pri(char c)
{
    if (c == '&')
        return 2;
    if (c == '|')
        return 1;
    return 0;
}

string get_pls(string s)
{
    string ans;
    stack<char> sta;
    for (char c : s)
    {
        switch (c)
        {
        case '0':
        case '1':
            ans += c;
            break;

        case '(':
            sta.push(c);
            break;

        case ')':
            while (!sta.empty() && sta.top() != '(')
            {
                ans += (sta.top());
                sta.pop();
            }
            if (!sta.empty())
                sta.pop();
            break;

        case '&':
        case '|':
            while (!sta.empty() && sta.top() != '(' && get_pri(sta.top()) >= get_pri(c))
            {
                ans += (sta.top());
                sta.pop();
            }
            sta.push(c);
            break;

        default:
            break;
        }
    }
    while (!sta.empty())
    {
        ans += (sta.top());
        sta.pop();
    }
    return ans;
}

struct Node
{
    int l = 0; // left
    int r = 0; // right
    char val = '|';
} nodes[1000005];

int nodes_cnt = 0;

void create_node(char val, int l, int r)
{
    nodes[++nodes_cnt].val = val;
    nodes[nodes_cnt].l = l;
    nodes[nodes_cnt].r = r;
}

int build_tree()
{
    stack<int> sta;
    // queue<Node> q;
    for (auto c : poland_s)
    {
        if (isdigit(c))
        {
            create_node(c, 0, 0);
        }
        else
        {
            // op
            int r = sta.top();
            sta.pop();
            int l = sta.top();
            sta.pop();

            create_node(c, l, r);
        }
        sta.push(nodes_cnt);
    }
    return sta.top();
}

int and_ans;
int or_ans;

int dfs(int u)
{
    char cur = nodes[u].val;

    if (isdigit(cur))
        return cur - '0';

    int retl = dfs(nodes[u].l);
    if (cur == '&' && retl == 0)
    {
        and_ans++;
        return 0;
    }
    if (cur == '|' && retl == 1)
    {
        or_ans++;
        return 1;
    }
    int retr = dfs(nodes[u].r);
    // cout << nodes[u].val;
    // cout << endl;
    if (cur == '&')
        return retl & retr;
    return retl | retr;
}

int main(int argc, char const *argv[])
{
    cin >> s;
    poland_s = get_pls(s);
    // cout << poland_s << endl;
    int u = build_tree();
    int ret = dfs(u); // u-> root
    cout << ret << endl;
    cout << and_ans << " " << or_ans << endl;

    return 0;
}
