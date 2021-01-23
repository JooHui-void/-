#include <stdio.h>
#include <stdlib.h>

int sort(const void *a,const void *b){
	if(*(int *)a>*(int *)b)
	return 1;
	else if(*(int *)a<*(int *)b)
	return -1;
	else 
	return 0;
}

int arr[25][25];
int cc;
void dfs(int a,int b){
	arr[a][b]=0;
	cc++;
	if(a<25&&arr[a+1][b]==1)
	dfs(a+1,b);
	if(b<25&&arr[a][b+1]==1)
	dfs(a,b+1);
	if(a>0&&arr[a-1][b]==1)
	dfs(a-1,b);
	if(b>0&&arr[a][b-1]==1)
	dfs(a,b-1);
}

int main(){
	

	int num;
	int count=0;
	int dangi[1000];
	scanf("%d",&num);
	for(int i=0;i<num;i++){
		for(int j=0;j<num;j++){
			scanf("%1d",&arr[i][j]);
		}
	}
	
	for(int i=0;i<num;i++){
		for(int j=0;j<num;j++){
			if(arr[i][j]==1){
				cc=0;

				dfs(i,j);
				dangi[count++]=cc;
			}
		}
	}
	qsort(dangi,count,sizeof(int),sort);
	printf("%d\n",count);
	for(int i=0;i<count;i++){
		printf("%d\n",dangi[i]);
	}

}
