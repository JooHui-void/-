#include <stdio.h>
#include <string.h>

char stack[100003];
int top=-1;

void push(char a){
	top++;
	stack[top]=a;
}

int main(){
	int num;
	char arr[100003];
	scanf("%d",&num);
	int sum=0;

	for(int i=0;i<num;i++){
		top=-1;
		scanf("%s",arr);
		for(int j=0;j<strlen(arr);j++){
			if(top==-1){
				push(arr[j]);
			}else{
				if(stack[top]!=arr[j]){
					push(arr[j]);
				}else{
					top--;
				}
			}
		}
		if(top==-1){
			sum++;
		}

	}
	printf("%d",sum);
} 
