#include <bits/stdc++.h>
using namespace std;

enum OP
{
    ADD,
    SUB,
    MUL
};

int main(int argc, char const *argv[])
{
    int a, b, c, d;
    int ta, tb, tc, td;
    cin >> a >> b >> c >> d;
    ta = a, tb = b, tc = c, td = d;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int t = 0;
            switch (i)
            {
            case ADD:
                t = ta + tb;
                break;
            case SUB:
                t = ta - tb;
                break;
            case MUL:
                t = ta * tb;
                break;

            default:
                break;
            }
            int ans = 0;
            switch (j)
            {
            case ADD:
                ans = t + tc;
                break;
            case SUB:
                ans = t - tc;
                break;
            case MUL:
                ans = t * tc;
                break;

            default:
                break;
            }
            if (ans == d)
            {
                cout << "Yes" << endl;
                return 0;
            }
            ta = a, tb = b, tc = c, td = d;
        }
    }
    cout << "No" << endl;

    return 0;
}
