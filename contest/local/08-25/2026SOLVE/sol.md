### A

玻璃罩要框住所有的试剂，但是在罩子的边界不视为在罩内。所以先计算出最大和最小的 $x$ 坐标和 $y$ 坐标，然后将最小坐标 $−1$，最大坐标 $+1$ 即为罩子四个点的坐标。

```cpp
#include<bits/stdc++.h>
#define int long long
using namespace std;

signed main()
{
	int n;
	cin>>n;
	int mxx=-1e9, mxy=-1e9, minx=1e9, miny=1e9;
	for(int i=1;i<=n;i++)
	{
		int x, y;
		cin>>x>>y;
		mxx=max(mxx,x);
		minx=min(minx,x);
		mxy=max(mxy,y);
		miny=min(miny,y);
	}
	cout<<minx-1<<" "<<miny-1<<"\n"<<mxx+1<<" "<<mxy+1;
}
```

### B

注意到数据范围很小，因此从每个格子开始向八个方向搜索，搜索时记录一下有没有转弯即可，主要难点在于代码的实现

```cpp
#include<bits/stdc++.h>
using namespace std;

const int X[]={1,0,-1,0};
const int Y[]={0,1,0,-1};
const int X2[]={1,-1,-1,1};
const int Y2[]={1,1,-1,-1};
string s;
int n,m,ans;
char a[105][105];

void dfs(int dep,int x,int y,int hd,bool f=0)
//f=0表示还没有转弯
//f=1表示已经转弯过了 
{
	if(dep==s.size())
	{
		//cerr<<x<<' '<<y<<endl;
		ans++;
		return;
	}
	//不转弯
	//转弯 
	int x_=x+X[hd],y_=y+Y[hd];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&a[x_][y_]==s[dep]) dfs(dep+1,x_,y_,hd,f);//姝ｅ父璧帮紝涓嬪悓
	x_=x+X[(hd+1)%4];
	y_=y+Y[(hd+1)%4];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&dep>1&&!f&&a[x_][y_]==s[dep]) dfs(dep+1,x_,y_,(hd+1)%4,1);//鍙宠浆 90掳锛屼笅鍚?
	x_=x+X[(hd+3)%4];
	y_=y+Y[(hd+3)%4];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&dep>1&&!f&&a[x_][y_]==s[dep]) dfs(dep+1,x_,y_,(hd+3)%4,1);//宸﹁浆 90掳锛屼笅鍚?
}

void dfs2(int dep,int x,int y,int hd,bool f=0)
{
	if(dep==s.size())
	{
		//cerr<<x<<' '<<y<<endl;
		ans++;
		return;
	}
	int x_=x+X2[hd],y_=y+Y2[hd];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&a[x_][y_]==s[dep]) dfs2(dep+1,x_,y_,hd,f);
	x_=x+X2[(hd+1)%4];
	y_=y+Y2[(hd+1)%4];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&dep>1&&!f&&a[x_][y_]==s[dep]) dfs2(dep+1,x_,y_,(hd+1)%4,1);
	x_=x+X2[(hd+3)%4];
	y_=y+Y2[(hd+3)%4];
	if(x_>0&&y_>0&&x_<=n&&y_<=m&&dep>1&&!f&&a[x_][y_]==s[dep]) dfs2(dep+1,x_,y_,(hd+3)%4,1);
}

int main()
{
	cin>>s>>n>>m;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			cin>>a[i][j];
		}
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(a[i][j]==s[0])
			{
				dfs(1,i,j,0);
				dfs(1,i,j,1);
				dfs(1,i,j,2);
				dfs(1,i,j,3);
				dfs2(1,i,j,0);
				dfs2(1,i,j,1);
				dfs2(1,i,j,2);
				dfs2(1,i,j,3);
			}
		}
	}
	cout<<ans;
 	return 0;
}
```

### C

遍历压缩字符串，解析出每个字符及其对应的重复次数，并计算字符串的总长度 $sum$ 。通过 $c\mod sum$ 找到第 $c$ 个字符在原始字符串中的位置。再次遍历压缩字符串，累加字符的重复次数，直到找到第 $c\mod sum$ 个字符。



```cpp
#include <bits/stdc++.h>
using namespace std;
long long sum, c, ans;
string s;
int main() {
    cin >> s >> c;
    for (int i = 0; i < s.length(); ) {
        char ch = s[i++]; 
        long long cc = 0;
        //字符是字母的时候 
        //字符串转十进制 
        //9  cc=9
        //7  cc=cc*10+7=97
        //1  cc=cc*10+1=971
        //2  cc=cc*10+2=9712
        while (i < s.length() && isdigit(s[i])) {
            cc = cc * 10 + (s[i] - '0'); 
            i++;
        }
        sum += cc;
    }
//    c=(c-1)%sum+1;
    //sum刚好是c的倍数的时候,sum->0 
    c %= sum;
    for (int i = 0; i < s.length(); ) {
        char ch = s[i++];
        long long cc = 0;
        while (i < s.length() && isdigit(s[i])) {
            cc = cc * 10 + (s[i] - '0'); 
            i++;
        }
        if (ans + cc > c) {
            cout << ch << endl;
            return 0;
        }
        ans += cc;
    }
    return 0;
}
```

### D

如果已经分好了队，对于每一队来说，最后吃完的人吃完的时间是
$$
\max_{i}\sum_{j=1}^{i}a_j+b_i
$$
一定是按 $b$ 降序排序更优，证明如下：

设 $a$ 的前缀和为 $s$ ，假设最后排序方案存在 $(i,j)$ ，其中 $b_i<b_j,i<j$ ，那么

- $i$ 吃完的时间是 $s_{i-1}+a_i+b_i$ 
- $j$ 吃完的时间是 $s_{j-1}+a_j+b_j$ 

如果交换 $(i,j)$ ，则

- $i$ 吃完的时间是 $s_{j-1}+a_i+b_i$
- $j$ 吃完的时间是 $s_{i-1}+a_j+b_j$ 

容易发现前种情况两者取 max 不小于后种情况的两者取 max ，因此交换 $
(i,j)$ 一定不劣。

因此，我们将人按照 $b$ 降序排序，设 $f_{i,j}$ 表示考虑前 $i$ 个人，第一队已经分配进去的人 $a$ 的和是 $j$ ，转移如下
$$
f_{i,j}=\min(\max(j+b_i,f_{i-1,j}),\max(s_i-j+b_i,f_{i-1,j}))
$$
可以去掉第一维，时间复杂度 $O(n^2V)$ 。

```cpp
#include<bits/stdc++.h>
using namespace std;
int n,s,f[250005];
struct node{
	int a,b;
}t[505];
bool cmp(node x,node y){
	return x.b>y.b;
}
int main(){
	cin>>n;
	for(int i=1;i<=n;i++)cin>>t[i].a>>t[i].b;
	sort(t+1,t+n+1,cmp);
	memset(f,0x3f,sizeof(f));
	f[0]=0;
	for(int i=1;i<=n;i++){
		s+=t[i].a;
		for(int j=s;j>=0;j--){
            f[j]=max(f[j],s-j+t[i].b);
			if(j>=t[i].a)f[j]=min(f[j],max(j+t[i].b,f[j-t[i].a]));
		}
	}
	int ans=1e9;
	for(int i=0;i<=s;i++)ans=min(ans,f[i]);
	cout<<ans;
}
```

### E

### 30pts

$2^N$ 枚举所有情况进行判定，时间复杂度为 $O(2^N)$

### +30pts

对于 $0 \leq a_i \leq 3$，$f[i][j][k]$ 表示考虑了序列的前 $i$ 个数，其中 $A$ 集合中所有元素的或运算为 $j$，$B$ 集合中所有元素的或运算为 $k$ 的方案数，其中 $j,k \in [0,3]$，背包进行转移，时间复杂度为 $O(N)$。

### 100pts

对于二进制下某一位 $k$，若满足第 $k$ 位上为 $1$ 的 $a_i$ 个数不为 $0$ 个，那么所有第 $k$ 位上为 $1$ 的数不能划分在同一个集合。

例如对于 $\{(101)_2,(100)_2,(001)_2,(111)_2 \}$ 这三个数，$\{(101)_2,(001)_2,(111)_2\}$ 一定不能被划分在同一个集合，否则另一个集合的最低位一定为 $0$，不满足题设条件。

所有第 $k$ 位上为 $1$ 的数不能划分在同一个集合并不好处理，我们考虑其相反面进行容斥，所有第 $k$ 位上为 $1$ 的数都应当划分在同一个集合并不好处理

考虑容斥，强制钦定第 $k$ 位上为 $1$ 的数划分在同一个集合。

现在有若干数强制钦定在同一个集合，将这些数用并查集连接，在同一个并查集里的数说明一定要被分配在一个集合里，考虑乘法原理，每个并查集的联通块都有 $2$ 种可能，即划分在 $A$ 或 $B$ ，那么方案数即为 $2^{并查集联通块个数}$，

容斥一共有 $O(2^{15})$ 种情况。

对于每一种情况，需要进行 $O(15 \times N)$ 的判断，但所有数最多合并到同一个并查集里，并查集部分的复杂度为$N \times \alpha(N)$，复杂度为$O(2^{15} \times (15\times N +N \times  \alpha(N)))$。

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=500+7,mod=998244353;
int f[N],a[N];
int g(int u){
	if(f[u]==u) return u; return f[u]=g(f[u]);
}
int pows(long long u,int v){
	long long ans=1;
	while(v>0){
		if(v&1) ans=ans*u%mod; u=u*u%mod,v=v>>1;
	}
	return ans;
}
int main(){
	int n,ans=0,D=0;
	cin>>n;
	for(int i=1;i<=n;i++) cin>>a[i],D|=a[i];
	//33=2^5+2^0
	//2^1
	for(int j=0;j<(1<<15);j++){
		if((D&j)!=j) continue; 
		for(int k=1;k<=n;k++) f[k]=k;//并查集初始化 
		for(int k=0;k<=14;k++){
			if(!(j&(1<<k))) continue;
			//(1<<k)这一位上为1
			//所有这一位上为1的数都要放到一个并查集里 
			int j=0;
			//所有满足 (1<<k)这一位上为1的数里面的第一个值 
			for(int c=1;c<=n;c++){
				if(a[c]&(1<<k)){
					if(!j) j=c;
					f[g(j)]=g(c);
					//j,c放到一个并查集里 
				}
			}
		}
		int s=0;
		for(int k=1;k<=n;k++) if(g(k)==k) s++;
		//求解j在二进制下有多少个1
		// +1
		// -1
		if(__builtin_popcount(j)%2==0) ans=(ans+pows(2,s))%mod;
		else ans=(ans-pows(2,s)+mod)%mod;
	}
	cout<<ans<<endl;
    return 0;
}

```

