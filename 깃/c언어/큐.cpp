#include <stdio.h>
#include <string.h>

int arr[2000000];
int top=-1;
int back=-1;
void push(int a){
	arr[++back]=a;
}
void pop(){
	if(top==back){
		printf("-1\n");
	}else{
	printf("%d\n",arr[++top]);
	}
	
}
void size(){
	printf("%d\n",back-top);
}
void empty(){
	if(back-top==0){
		printf("1\n");
	}else{
		printf("0\n");
	}
}
void front(){
	if(back-top==0){
		printf("-1\n");
	}else{
		printf("%d\n",arr[top+1]);
	}
}
void backk(){
	if(back-top==0){
		printf("-1\n");
	}else{
		printf("%d\n",arr[back]);
	}
}

int main(){
	int num;
	scanf("%d",&num);
	char word[7];
	int nu;
	for(int i=0;i<num;i++){
		scanf("%s",word);
		if(strcmp(word,"push")==0){
			scanf("%d",&nu);
			push(nu);
		}else if(strcmp(word,"size")==0){
			size();
		}else if(strcmp(word,"empty")==0){
			empty();
		}else if(strcmp(word,"front")==0){
			front();
		}else if(strcmp(word,"back")==0){
			backk();
		}else if(strcmp(word,"pop")==0){
			pop();
		}
	}
}
