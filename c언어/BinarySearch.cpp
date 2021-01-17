//1920
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sort(const void *a,const void *b){
	if(*(long long *)a>*(long long *)b){
		return 1;
	}else if(*(long long *)a<*(long long *)b){
		return -1;
	}else
	return 0;
}
int BinarySearch(int num,long long array[],long long target){
	int start=0;
	int end=num-1;
	int mid=(end+start)/2;
	while(end-start>=0){
		if(array[mid]==target){
			return 1;
		}else if(array[mid]<=target){
			start=mid+1;
		}else{
			end=mid-1;
		}
		mid=(end+start)/2;
	}
	return 0;
}

int main(){
	int num;
	scanf("%d",&num);
	long long *q=(long long *)malloc(sizeof(long long)*num);
	for(int i=0;i<num;i++){
		scanf("%lld",&q[i]);
	}
	qsort(q,num,sizeof(long long),sort);
	
	int nn;
	scanf("%d",&nn);
	long long ber;

	for(int i=0;i<nn;i++){
		scanf("%lld",&ber);
		printf("%d\n",BinarySearch(num,q,ber));
	}
}
