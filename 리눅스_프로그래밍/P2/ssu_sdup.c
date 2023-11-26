// 프롬프트 출력 및 내장명령어 호출을 위한 프로그램
#include "ssu_sdup.h"


char getstr[NAMESIZE];
int main(void){

	while(1){
		char tok[10][NAMESIZE]; // 명령어 한줄로 입력받음 
		printf("20202912> ");
		char c;
		int tt=0;
		fgets(getstr, NAMESIZE, stdin);
		while(getstr[tt]!='\n'){ // fgets 맨 뒤 개행문자 널문자로 교체 
			tt++;
		}
		getstr[tt]='\0';
		char tmp[10]; // 띄어쓰기 기준으로 토큰화 
		strcpy(tmp,getstr); 
		char *ptr = strtok(getstr," ");
		int cnt =0;
		
		while(ptr != NULL){ //띄어쓰기로 분리해서 저장
			strcpy(tok[cnt++],ptr);
			ptr = strtok(NULL, " ");
		}
		pid_t child1;
		int status;
		if(strcmp(tok[0],"fmd5")==0){ // fmd5 명령어 사용 
			if(cnt!=5){  //usage error 
				fprintf(stderr,"usage error\n");
				exit(1);
			}
			child1 = fork();
			if(child1 ==0){	 // 자식 프로세스 
				execl("./ssu_find-md5",tok[1],tok[2],tok[3],tok[4],(char*)0); // find-md5 exec 
				exit(0);
			}
			wait(&status);
			
		}else if(strcmp(tok[0], "fsha1")==0){ // fsha1 명령어 사용 
			
			if(cnt!=5){ // usage error 
				fprintf(stderr,"usage error\n");
				exit(1);
			}
			child1 = fork();
			if(child1 ==0){  // 자식 프로세스 
				execl("./find-sha1",tok[1], tok[2], tok[3], tok[4],(char *)0); // find-sha1 exec 
				exit(0);
			}
			wait(&status); // 부모 기다렸다 실행 
			continue;
		}else if(strcmp(tok[0], "exit")==0){ // exit 
			printf("Prompt End\n");
			exit(0);
		}else if(strcmp(tok[0],"\n")==0){
			continue;
		}else{ // help 출력 
			
			child1 = fork(); // 자식 프로세스 실행 
			if(child1 ==0){
				execl("./ssu_help",(char *)0); // help exec 
				exit(0);
			}
			wait(&status); // 부모 기다렸다 실행 
			//help();

		}


	}
	exit(0);
}

