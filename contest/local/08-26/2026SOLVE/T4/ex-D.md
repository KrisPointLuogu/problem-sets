```cpp
#include <bits/stdc++.h>
using namespace std;
const int N=1e3+7,S=(1<<12);
long long n,a[4][N],dp[N][S],w[8][12],col[S<<3][4];
int getcol(int t,int b,int x,int y){//在一个大小为3*b的网格中，压缩值为t，询问(x,y)格子上的颜色
//红色为+1,黑色为0,蓝色为-1 
	memset(w,-1,sizeof(w));
	for(int y=b;y>=1;y--){
		for(int x=3;x>=1;x--){
			w[x+2][y+2]=(t&1),t=t/2;//扩大一圈，避免越界 
		}
	}
	x+=2,y+=2;
	int flag=0;
	if(w[x][y]==w[x-1][y]&&w[x-1][y]==w[x-2][y]) flag=1;
	if(w[x][y]==w[x-1][y]&&w[x-1][y]==w[x+1][y]) flag=1;
	if(w[x][y]==w[x+1][y]&&w[x+1][y]==w[x+2][y]) flag=1;
	if(w[x][y]==w[x][y-1]&&w[x][y]==w[x][y+1]) flag=1;
	if(w[x][y]==w[x][y-1]&&w[x][y]==w[x][y-2]) flag=1;
	if(w[x][y]==w[x][y+1]&&w[x][y]==w[x][y+2]) flag=1;
	if(w[x][y]==w[x+1][y+1]&&w[x+1][y+1]==w[x+2][y+2]) flag=1;
	if(w[x][y]==w[x-1][y-1]&&w[x-1][y-1]==w[x+1][y+1]) flag=1;
	if(w[x][y]==w[x-1][y-1]&&w[x-1][y-1]==w[x-2][y-2]) flag=1;
	if(w[x][y]==w[x-1][y+1]&&w[x][y]==w[x-2][y+2]) flag=1;
	if(w[x][y]==w[x-1][y+1]&&w[x][y]==w[x+1][y-1]) flag=1;
	if(w[x][y]==w[x+1][y-1]&&w[x][y]==w[x+2][y-2]) flag=1;
	if(!flag) return 0;
	if(!w[x][y]) return 1;
	return -1;
}
int main(){
	freopen("chess.in","r",stdin);
	freopen("chess.out","w",stdout);
	cin>>n;
	for(int i=1;i<=3;i++)
		for(int j=1;j<=n;j++)
			cin>>a[i][j];
	if(n<=5){//n<=5暴力 
		long long ans=-1e18;
		for(int j=0;j<(1<<(3*n));j++){
			long long sum=0;
			for(int x=1;x<=3;x++)
				for(int y=1;y<=n;y++)
					sum+=getcol(j,n,x,y)*a[x][y];
			ans=max(ans,sum);
		}
		cout<<ans<<'\n'; return 0;
	}
	for(int j=0;j<(1<<15);j++)
		for(int k=1;k<=3;k++)
			col[j][k]=getcol(j,5,k,3);
	for(int i=4;i<=n;i++)
		for(int j=0;j<(1<<12);j++) dp[i][j]=-1e18;
	for(int j=0;j<(1<<12);j++){
		long long ans=0;
		for(int x=1;x<=3;x++)
			for(int y=1;y<=2;y++){
				ans+=getcol(j<<3,5,x,y)*a[x][y];
			}
		dp[4][j]=ans;//加上第一列第二列的答案 
	}
	for(int i=4;i<=n;i++)
		for(int j=0;j<(1<<12);j++){
			for(int k=0;k<8;k++){
				int t=((j&((1<<9)-1))<<3)+k;
				int s=(j<<3)+k;
				dp[i+1][t]=max(dp[i+1][t],col[s][1]*a[1][i-1]+col[s][2]*a[2][i-1]+col[s][3]*a[3][i-1]+dp[i][j]);
			}
		}
	long long ans=-1e18;
	for(int j=0;j<(1<<12);j++){
		long long sum=dp[n][j];
		for(int x=1;x<=3;x++)
			for(int y=4;y<=5;y++){
				sum+=getcol(j,5,x,y)*a[x][n+y-5];//加上第n-1列第n列的答案 
			}
		ans=max(ans,sum);
	}
	cout<<ans<<'\n';
    return 0;
}
```

