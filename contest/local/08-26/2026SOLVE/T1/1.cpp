string a[3005][3005];
unordered_map<string,int>F;
int n,m;
for(int i=0;i<n;i+=3){
	for(int j=0;j<m;j+=3){
		string s="";
		for(int x=0;x<=2;x++)
			for(int y=0;y<=2;y++)
				s+=a[i+x][j+y];
	}
} 
