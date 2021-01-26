#include <stdio.h>

void hanoi(int n,char from,char tmp,char to){
	if(n==1)
	printf("%c %c\n",from,to);
	else{
		hanoi(n-1,from,to,tmp);
		printf("%c %c\n",from,to);
		hanoi(n-1,tmp,from,to);
	}
}


int main(){
	int num;
	long double sum=1;
	scanf("%d",&num);
	for(int i=0;i<num;i++){
		sum=sum*2;
	}
	printf("%lf\n",sum-1);
	if(num<=20){
		hanoi(num,'1','2','3');
	}
	return 0;
}
