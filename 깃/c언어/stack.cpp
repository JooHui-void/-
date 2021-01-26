#include <stdio.h>
#include <string.h>

int top=-1;

void push(int a[],int num){
	top++;
	a[top]=num;
}
int empty(int a[]){
	if(top==-1)
	return 1;
	else
	return 0;
}
void size(int a[]){
	printf("%d\n",top+1);
}
void pop(int a[]){
	if(top==-1)
	printf("-1\n");
	else{
		printf("%d\n",a[top]);
		top--;	
	}

}
void ttop(int a[]){
	if(top==-1)
	printf("-1\n");
	else
	printf("%d\n",a[top]);
}
int main(){
	int arr[10000];
	int num;
	char sen[7];
	int kk;
	scanf("%d",&num);
	for(int i=0;i<num;i++){
		scanf("%s",sen);
		if(strcmp(sen,"push")==0){
			scanf("%d",&kk);
			push(arr,kk);
		}else if(strcmp(sen,"pop")==0){
			pop(arr);
		}else if(strcmp(sen,"size")==0){
			size(arr);
		}else if(strcmp(sen,"empty")==0){
			printf("%d\n",empty(arr));
		}else if(strcmp(sen,"top")==0){
			ttop(arr);
		}
	}
}
