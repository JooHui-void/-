#include "ssu_sfinder.h"

extern fileList* dups_list_h;
fileInfo *trash_list;
void sort_trash(int cate, int order);
void sec(struct tm *time, char *ymdt)
{
	sprintf(ymdt, "%04d-%02d-%02d 		%02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}

void print_trash_list(){
	
	
	fileInfo *move = trash_list->next;
	if(move == NULL){
		printf("Trash bin is empty\n");
		return;
	}
	int cnt =1;
	printf("      FILENAME		     SIZE	 DELETION DATE		DELETION TIME\n");
	while(move!=NULL){
		char time[100];
		sec(localtime(&move->trash_time),time);
		printf("[%-2d] %-40s %-10d %s\n",cnt++, move->path, move->statbuf.st_size, time);
		move = move->next;
	}
	printf("\n");
	
}
void strash(int argc, char *argv[]){
	int param_opt;
	optind = 0;
	opterr = 0;
	int category= 0;
	int order = 1;
	while((param_opt = getopt(argc, argv, "c:o:"))!=-1){ // 옵션 처리 
		switch(param_opt){
			case 'c' : // 정렬할 카테고리 
				if(!strcmp(optarg,"filename"))
					category = 0;
				else if(!strcmp(optarg,"size"))
					category = 1;
				else if(!strcmp(optarg,"date"))
					category = 2;
				else if(!strcmp(optarg,"time"))
					category = 3;
				break;
			case 'o' : // 정렬 기준 
				if(!strcmp(optarg,"-1"))
					order = -1;
				else
					order =1;
				break;
			
			case '?' :
				printf("unknowned option\n");
				return;
		}
	}
	//sort_trash(category, order);
	print_trash_list();
	
	
	
}
