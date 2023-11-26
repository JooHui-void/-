#include "ssu_sfinder.h"

char extension[ARGMAX]; // 확장자 저장 
long long minbsize, maxbsize; // 사이즈 범위 저장 
char target[PATHMAX]; // 목표 디렉토리 경로 
int thread_num;

extern fileList* dups_list_h;

int tokenize(char *input, char *argv[])
{
	char *ptr = NULL;
	int argc = 0;
	ptr = strtok(input, " ");

	while (ptr != NULL){
		argv[argc++] = ptr;
		ptr = strtok(NULL, " ");
	}
	return argc;
}

long long get_size(char *filesize) // 파일 사이즈 저장 함수 
{
	double size_bytes = 0;
	char size[STRMAX] = {0, };
	char size_unit[4] = {0, };
	int size_idx = 0;

	if (!strcmp(filesize, "~")) // default 값 사용 
		size_bytes = -1;
	else {
		for (int i = 0; i < strlen(filesize); i++){  // size 에러 처리 
			if (isdigit(filesize[i]) || filesize[i] == '.'){
				size[size_idx++] = filesize[i];
				if (filesize[i] == '.' && !isdigit(filesize[i + 1]))
					return SIZE_ERROR;
			}
			else {
				strcpy(size_unit, filesize + i);
				break;
			}
		}

		size_bytes = atof(size); // 실수로 변환 

		if (strlen(size_unit) != 0){
			if (!strcmp(size_unit, "kb") || !strcmp(size_unit, "KB"))
				size_bytes *= KB;
			else if (!strcmp(size_unit, "mb") || !strcmp(size_unit, "MB"))
				size_bytes *= MB;
			else if (!strcmp(size_unit, "gb") || !strcmp(size_unit, "GB"))
				size_bytes *= GB;
			else
				return SIZE_ERROR;
		}
	}

	return (long long)size_bytes;
}

void get_path_from_home(char *path, char *path_from_home) // 홈디렉토리 포함 경로 얻는 옵션 (~포함 경로, 저장 위치)
{
	char path_without_home[PATHMAX] = {0,};
	char *home_path;

	home_path = getenv("HOME");

	if (strlen(path) == 1){ // ~ 일때 
		strncpy(path_from_home, home_path, strlen(home_path)); // 홈 디렉토리 경로만 붙여줌 
	}
	else {
		strncpy(path_without_home, path + 1, strlen(path)-1); // ~ 제외 뒤의 경로 path_without_home에 넣음 
		sprintf(path_from_home, "%s%s", home_path, path_without_home);  // 둘을 붙여줌 
	}
}

void fmd5(int argc, char *argv[]){
	extension[0]='\0';
	minbsize =0;
	maxbsize = 0;
	thread_num = 1;
	target[0]='\0';

	int param_opt;
	optind = 0;
	opterr = 0;
	if(argc<9){ // argv 다 들어왔는지 검사 
		fprintf(stderr, "usage error!");
		exit(1);
	}
	while((param_opt = getopt(argc, argv, "e:l:h:d:t:"))!=-1){ // 옵션 처리 
		switch(param_opt){
			case 'e' :  // 확장자 
				if (strchr(optarg, '*') == NULL){ // *가 없을 시 에러 처리 
					printf("ERROR: [FILE_EXTENSION] should be '*' or '*.extension'\n");
					return;
				}
				if (strchr(optarg, '.') != NULL){ // 뒤에 확장자 존재할 시 
					strcpy(extension, get_extension(optarg)); // extention 에 확장자 저장 

					if (strlen(extension) == 0){ // 확장자 안들어오면 에러 
						printf("ERROR: There should be extension\n");
						return;
					}
				}

				break;
			case 'l' : // 최소 파일 크기
				minbsize = get_size(optarg);
				if(minbsize == -1)
					minbsize = 0;
				break;
			case 'h' : // 최대 파일 크기 
				maxbsize = get_size(optarg);


				break;
			case 'd' : // 디렉토리 경로 
				if (strchr(optarg, '~') != NULL) // 홈 경로 있을 시 
					get_path_from_home(optarg, target); // 홈 디렉토리 포함 경로로 변경 
				else{ // 홈 경로 없을 시 
					if (realpath(optarg, target) == NULL){ //target에 절대경로 넣어줌 
						printf("ERROR: [TARGET_DIRECTORY] should exist\n");
						return;
					}
				}

				break;
			case 't' : // 탐색 스레드 개수 
				thread_num = atoi(optarg);
				break;
			case '?' :
				printf("unknowned option\n");
				return;
		}
	}
	if (minbsize == SIZE_ERROR || maxbsize == SIZE_ERROR){ // 사이즈 에러 처리 
		printf("ERROR: Size wrong");
		return;
	}

	if (maxbsize != -1 && minbsize > maxbsize){ // 최소값이 최대값보다 더 클때 
		printf("ERROR: [MAXSIZE] should be bigger than [MINSIZE]\n");
		return;
	}
	get_same_size_files_dir();
	struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL); // 시간 측정 시작 
	make_list(); // 중복 파일 생성
	find_duplicates();  // 링크드리스트 제작 
	remove_no_duplicates();
	
	if (dups_list_h->next == NULL){ // 중복 파일 없을 시 
		printf("No duplicates in %s\n", target);
	}
	else 
		filelist_print_format(dups_list_h);
		
		
	gettimeofday(&end_t, NULL);
	end_t.tv_sec -= begin_t.tv_sec;
	if(end_t.tv_usec < begin_t.tv_usec){
	end_t.tv_sec--;
		end_t.tv_usec +=1000000;
	}

	end_t.tv_usec -= begin_t.tv_usec;
	printf("Searching time: %ld:%06ld(sec:usec)\n\n",end_t.tv_sec, end_t.tv_usec);
	get_trash_path();
	if(dups_list_h->next != NULL){
	while(1){
		char input[STRMAX];
		char *argv[ARGMAX];
		int argc =0;
		printf(">> ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input)-1] = '\0';


		argc = tokenize(input, argv);
		argv[argc] = (char *)0;	
		
		if(!strcmp(argv[0], "exit")) // exit
			break;
		if(!strcmp(argv[0],"delete")){ // delete 명령어 실행시 
		
			sdelete(argc,argv);
			filelist_print_format(dups_list_h);
		}else{ // 예외
			
		}
		
	}
	}
	printf("Prompt End\n");
}
