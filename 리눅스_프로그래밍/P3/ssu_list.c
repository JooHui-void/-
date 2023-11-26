#include "ssu_sfinder.h"
int list_type = 0;
int category = 1;
int order =1;

extern fileList* dups_list_h;
void slist(int argc, char *argv[]){
	int param_opt;
	optind = 0;
	opterr = 0;
	list_type = 0;
	category = 1;
	order =1;
	while((param_opt = getopt(argc, argv, "l:c:o:"))!=-1){ // 옵션 처리 
		switch(param_opt){
			case 'l' :  // 동일한 파일 세트
				if(!strcmp(optarg,"filelist"))
					list_type = 1;
				break;
			case 'c' : // 정렬할 카테고리 
				if(!strcmp(optarg,"filename"))
					category = 0;
				else if(!strcmp(optarg,"uid"))
					category = 2;
				else if(!strcmp(optarg,"gid"))
					category = 3;
				else if(!strcmp(optarg,"mode"))
					category = 4;
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
	sortList(category,list_type, order);
	filelist_print_format(dups_list_h);
}
