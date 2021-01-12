#include <stdio.h>
#include <string.h>

char stack[51];
int top=-1;
int main(){
	char arr[102];
	int len;
	int ans[1000];
	int a=0;
	while(1){
		top=-1;
		fgets(arr,100,stdin);
//		getchar();
//		fputs(arr,stdout);
		if(strcmp(arr,".\n")==0){
			break;
		}

		len=strlen(arr);

		for(int i=0;i<len;i++){
			if(arr[i]=='('||arr[i]=='['){
				top++;
				stack[top]=arr[i];
			}else if(arr[i]==')'||arr[i]==']'){
				if(arr[i]==')'){
					if(stack[top]=='('){
						top--;
					}else{
						top=999;
					}
				}else if(arr[i]==']'){
					if(stack[top]=='['){
						top--;
					}else{
						top=999;
					}
				}
				
			}
			
		}
		if(top==-1){
			ans[a++]=1;
		}else{
			ans[a++]=0;
		}
		
	}
	for(int i=0;i<a;i++){
		if(ans[i]==1){
			printf("yes\n");
		}else if(ans[i]==0){
			printf("no\n");
		}
	}
}
