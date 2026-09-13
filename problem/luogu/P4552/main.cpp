#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
LL n,c,p,q,a[100010];
int main()
{
	cin>>n;
	for(int i=1;i<=n;i++)
	{
		scanf("%lld",&a[i]);
	}
	for(int i=2;i<=n;i++)
	{
		c=a[i]-a[i-1];
		if(c>0)
		{
			p+=c;
		}
		else 
		q-=c;
	}
	LL ans1=max(p,q);
	LL ans2=abs(p-q)+1;
	cout<<ans1<<endl<<ans2;
	return 0;
}
