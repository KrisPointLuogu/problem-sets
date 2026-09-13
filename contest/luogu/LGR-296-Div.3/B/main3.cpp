#include <bits/stdc++.h>
using namespace std;

/*
思路：以每个 cube 数为界分割成队列，维护队列指针，逐轮输出。
队列 k = [k^3, (k+1)^3 - 1]，位置 q 被删后推进到 q + k（若 q+k 仍在队列 k 内）。
同一队列可能有多条交错子链（如 Q2 的 8,10,12,... 与 9,11,13,...），
所以每轮维护"当前指针集合"（所有子链的队头），按顺序输出。
*/

const int maxn = 1e6 + 5;
const int CUBE_MAX = 105; // n <= 1e6 时最大的 k 满足 k^3 <= n 是 100

int point[maxn];        // 指针集合：初始为所有 cube，之后为每轮子链的队头
int vis[maxn];          // vis[p] = 1 表示 p 是完全立方数（第 0 轮删除）
int pointvis[maxn];     // 下一轮指针集合的缓冲区
long long lo[CUBE_MAX]; // lo[k] = k^3 - k，判定 q 能否推进到队列 k

int n;
vector<long long> a; // 1 下标数列 a[1..n]

void read_input()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    a.assign(n + 1, 0);
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
}

// 标记立方数位置；初始化 point[k] = k^3（队列 k 的队头）；返回最大 k 满足 k^3 <= n
int init_cubes()
{
    int maxcube = 0;
    for (long long k = 1; k * k * k <= n; k++)
    {
        int cubePos = (int)(k * k * k);
        point[k] = cubePos;
        vis[cubePos] = 1;
        maxcube = k;
    }
    return maxcube;
}

// lo[k] = k^3 - k；位置 q 能推进到队列 k 当且仅当 lo[k] <= q <= lo[k+1]
void precompute_lo(int maxcube)
{
    for (long long k = 1; k <= maxcube + 1; k++)
    {
        lo[k] = k * k * k - k;
    }
}

// 把 q 的下一轮后继 q+k 写入 pointvis；合法 k 只可能是 k0-1, k0, k0+1
void collect_next(int q, int maxcube, int &ncnt)
{
    int k0 = (int)(upper_bound(lo + 1, lo + maxcube + 1, (long long)q) - lo) - 1;
    for (int k = k0 - 1; k <= k0 + 1; k++)
    {
        if (k < 1 || k > maxcube)
        {
            continue;
        }
        if (lo[k] <= q && q <= lo[k + 1])
        {
            long long nextPos = (long long)q + k;
            if (nextPos <= n && !vis[nextPos])
            {
                pointvis[ncnt++] = (int)nextPos;
            }
        }
    }
}

// 核心流程：第 0 轮为所有 cube，之后每轮取出指针集合输出并推进到下一轮，
// 直到指针集合为空；返回 ans，ans[r] 为第 r 轮删掉的数（按原数列顺序）
vector<vector<int>> gen_rounds(int maxcube)
{
    vector<vector<int>> ans;

    int qcnt = 0; // 当前轮指针个数
    for (int k = 1; k <= maxcube; k++)
    {
        point[qcnt++] = point[k]; // 第 0 轮指针集合 = 所有 cube
    }

    while (qcnt > 0)
    {
        vector<int> arr; // 本轮删掉的数
        int ncnt = 0;    // 下一轮指针个数

        for (int i = 0; i < qcnt; i++)
        {
            int q = point[i];
            arr.push_back(a[q]);
            collect_next(q, maxcube, ncnt);
        }

        ans.push_back(arr);
        for (int i = 0; i < ncnt; i++)
        {
            point[i] = pointvis[i]; // 把下一轮指针搬到 point[0..ncnt)
        }
        qcnt = ncnt;
    }

    return ans;
}

void print_ans(vector<vector<int>> ans)
{
    cout << ans.size() << '\n';
    for (auto row : ans)
    {
        for (size_t j = 0; j < row.size(); j++)
        {
            cout << row[j] << " ";
        }
        cout << '\n';
    }
}

int main()
{
    read_input();
    int maxcube = init_cubes();
    precompute_lo(maxcube);
    vector<vector<int>> ans = gen_rounds(maxcube);
    print_ans(ans);
    return 0;
}
