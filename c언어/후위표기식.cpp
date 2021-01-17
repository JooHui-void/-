#include <stdio.h>
#include <string.h>
#define string_size 100
int top=-1;

void push(char a[],char num){
	top++;
	a[top]=num;
}

char pop(char a[]){
	char re;
	re=a[top];
	top--;
	return re;
}

char ttop(char a[]){

	return a[top];
}

int main(){
	scanf("%s");
	char arr[string_size];
	char stack[string_size];
	scanf("%s",arr);
	char retu=0;
	int count=0;
	for(int i=0;i<strlen(arr);i++){
		if(arr[i]<='Z'&&arr[i]>='A'){
			printf("%c",arr[i]);
			if(count==1){
				while(top!=-1){
					retu=pop(stack);
					printf("%c",retu);
				}
				count=0;
			}
		}else if(arr[i]=='('){
			push(stack,'(');
		}else if(arr[i]==')'){
			retu=pop(stack);
			while(retu!='('){
				printf("%c",retu);
				retu=pop(stack);
				
			}
		}else if(arr[i]=='*'||arr[i]=='/'){
			if(top==-1){
				push(stack,arr[i]);
			}else if(ttop(stack)=='*'||ttop(stack)=='/'){
				printf("%c",pop(stack));
				push(stack,arr[i]);
			}else{
				push(stack,arr[i]);
			}
		}else if(arr[i]=='+'||arr[i]=='-'){
			if(top==-1){
				push(stack,arr[i]);
			}else if(ttop(stack)=='*'||ttop(stack)=='/'){
				while(top!=-1){
					retu=pop(stack);
					printf("%c",retu);
				}
				push(stack,arr[i]);
			}else if(ttop(stack)=='+'||ttop(stack)=='-'){
				printf("%c",pop(stack));
				push(stack,arr[i]);
			}else{
				push(stack,arr[i]);
			}
		}
		
	}
	while(top!=-1){
		retu=pop(stack);
		printf("%c",retu);
	}
}
