#include "ssu_sfinder.h"
void fmd5(int argc, char**);
void help();
void strash(int argc, char *argv[]);
int tokenize(char *input, char *argv[]);
extern fileInfo *trash_list;
int main(){
	trash_list = (fileInfo*)malloc(sizeof(fileInfo));
	while(1){
		char input[STRMAX];
		char *argv[ARGMAX];
		int argc =0;
		printf("20202912> ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';


		argc = tokenize(input, argv);
		argv[argc] = (char *)0;	
		if(argc == 0)
			continue;
		if(!strcmp(argv[0], "exit")) // exit
			break;
		if(!strcmp(argv[0],"fmd5")){ // fmd5 명령어 실행시 
			fmd5(argc,argv);
		}else if(!strcmp(argv[0],"list")){ // list 함수 호출 
			slist(argc,argv);
		}else if(!strcmp(argv[0],"trash")){ // trash 함수 호출 
			strash(argc,argv);
		}else if(!strcmp(argv[0], "help")){ // help 함수 호출
			help();
		}else if(!strcmp(argv[0], "restore")){ // help 함수 호출
			int n = atoi(argv[1]);
			restore(n);
		}else{ // restore 함수 호출 
			help();
		}
	}
	
	printf("Prompt End\n");
	
}
