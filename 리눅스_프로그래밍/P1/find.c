#include "ssu_sindex.h"
#include <errno.h>

char *real_find_path;
char cur_dir[255];
struct stat st;
long long find_size;
int filenum =0;
char name_tmp[10];
char buf[10];
int year;
int month;
int day;
int hour;
int min;
char all_time[45];
struct tm *time_info;
char real_path[300];
char name_list[20][200];
int findlen=0;
int findlen0=0;
int dirgap =0;
void TakeTime(){
	year = time_info->tm_year;
	month = time_info->tm_mon;
	day = time_info->tm_mday;
	hour = time_info->tm_hour;
	min = time_info->tm_min;
}
void MakeTime(struct stat *st){
	time_info = gmtime(&st->st_atime);
	TakeTime();
	sprintf(all_time,"%02d-%02d-%02d %02d:%02d ",year-100,month,day,hour,min);

	time_info = gmtime(&st->st_mtime);
	TakeTime();
	sprintf(all_time,"%s%02d-%02d-%02d %02d:%02d ",all_time,year-100,month,day,hour,min);

	time_info = gmtime(&st->st_ctime);
	TakeTime();
	sprintf(all_time,"%s%02d-%02d-%02d %02d:%02d ",all_time,year-100,month,day,hour,min);
}
void PrintMode(int a){
	if(S_ISDIR(a)){
		name_tmp[0]='d';
	}else{
		name_tmp[0]='-';
	}
	int mask = 256;
	for(int i=0;i<9;i++){
		if(!(mask&a)){
			name_tmp[i+1]='-';
		}else{
			if(i%3==0){
				name_tmp[i+1]='r';
			}else if(i%3==1){
				name_tmp[i+1]='w';
			}else{
				name_tmp[i+1]='x';
			}
		}
		mask=mask>>1;
	}

}
void PrintSame(int x,int y){
	if(x==y){
		printf("%d",x);
	}else{
		printf("%d,%d",x,y);
	}
}
void PrintAll(struct stat *st,long long size,char *path){
	MakeTime(st);
	realpath(path,real_path);
	PrintMode(st->st_mode);
	//printf("%s %s %d\n",real_find_path,realpath,strcmp(real_find_path,realpath));
	if(filenum==0||strcmp(real_find_path,real_path)!=0){

		strcpy(name_list[filenum],path);
		printf("%-5d %-5lld %s %-6lld %-5ld %-3ld %-3ld %s %s\n",filenum++,(long long)size,name_tmp,(long long)st->st_blocks,(long)st->st_nlink,(long)st->st_uid,(long)st->st_gid,all_time,real_path);
	}
}
long long SizeSum(char *filename){
	long long size =0;
	struct dirent *file;
	struct stat st1;
	DIR *dir;
	char name[100];
	if((dir=opendir(filename))==NULL){
		printf("error1");
		return -1;
	}

	while((file = readdir(dir))!=NULL){
		if(strcmp(file->d_name,".")==0||strcmp(file->d_name,"..")==0)
			continue;

		sprintf(name,"%s/%s",filename,file->d_name);
		stat(name,&st1);

		if(S_ISDIR(st1.st_mode)){
			size+=SizeSum(name);
		}else{
			int *file;
			char name[100]="";
			size+=(long long)st1.st_size;
		}

	}
	closedir(dir);
	return size;

}

int option[4];
int realcnt=0;
int cmpcnt=0;
char realname[40][100];
char cmpname[40][100];

void SaveName(char *rname,int x){
	DIR *dir;
	struct dirent *file;
	struct stat st;
	if((dir = opendir(rname))==NULL){
		printf("dir open fail!\n");
		return -1;
	}
	char name[100]="";
	if(x==0){ //realfile 인 경우 
		strcpy(realname[realcnt++],rname);
		while((file=readdir(dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s\0",rname,file->d_name);
			stat(name,&st);
			if(S_ISDIR(st.st_mode)){
				SaveName(name,0);
			}else{
				strcpy(realname[realcnt++],name);
			}
		}
	}else{ //cmpfile인 경우 
		strcpy(cmpname[cmpcnt++],rname);
		while((file=readdir(dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s\0",rname,file->d_name);
			stat(name,&st);
			if(S_ISDIR(st.st_mode)){
				SaveName(name,1);
			}else{
				strcpy(cmpname[cmpcnt++],name);
			}
		}
	}
}
int diffcheck=0;
void DirDiff(int x){
	DIR *real_dir;
	DIR *cmp_dir;
	struct dirent *file;
	struct stat st;
	realcnt=0;
	cmpcnt=0;
	char tmp[100]="";
	char name[100]="";
	dirgap = strlen(name_list[x])-strlen(name_list[0]);
	//printf("%d|%d\n",index,strlen(buf));
	if((real_dir=opendir(name_list[x]))==NULL){ // 찾은게 real이다.
		printf("file error1!\n");
		return -1;
	}

	if((cmp_dir=opendir(name_list[0]))==NULL){ // 현 디렉토리에 있는게 cmp 이다. 
		printf("file error!\n");
		return -1;
	}
	if(option[3]){ // 재귀 사용 O

		while((file=readdir(real_dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s",name_list[x],file->d_name);
			stat(name,&st);
			if(S_ISDIR(st.st_mode)){
				SaveName(name,0);
			}else{
				strcpy(realname[realcnt++],name);
			}
		}
		while((file=readdir(cmp_dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s",name_list[0],file->d_name);
			stat(name,&st);
			if(S_ISDIR(st.st_mode)){
				SaveName(name,1);
			}else{
				strcpy(cmpname[cmpcnt++],name);
			}
		}
	}else{//재귀 사용 X
		while((file=readdir(real_dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s\0",name_list[x],file->d_name);
			strcpy(realname[realcnt++],name);
		}
		while((file=readdir(cmp_dir))!=NULL){
			name[100]="";
			if(!strcmp(file->d_name,".")||!strcmp(file->d_name,".."))
				continue;
			sprintf(name,"%s/%s\0",name_list[0],file->d_name);
			strcpy(cmpname[cmpcnt++],name);
		}
	}
	for(int i=realcnt-1;i>0;i--){ // 파일 이름 정렬 
		for(int j=0;j<i;j++){
			if(strcmp(realname[j],realname[j+1])>0){
				strcpy(tmp,realname[j]);
				strcpy(realname[j],realname[j+1]);
				strcpy(realname[j+1],tmp);
			}
		}
	}
	for(int i=cmpcnt-1;i>0;i--){ // 파일 이름 정렬 
		for(int j=0;j<i;j++){
			if(strcmp(cmpname[j],cmpname[j+1])>0){
				strcpy(tmp,cmpname[j]);
				strcpy(cmpname[j],cmpname[j+1]);
				strcpy(cmpname[j+1],tmp);
			}
		}
	}

	int cmpptr=0;
	int realptr=0;
	int slash =0;

	while(cmpptr<cmpcnt||realptr<realcnt){ //투포인터로 비교
		if(cmpptr==cmpcnt){
			while(realptr<realcnt){
				for(int i=0;i<strlen(realname[realptr]);i++){
					if(realname[realptr][i]=='/'){
						slash = i;
					}
				}
				strncpy(tmp,&realname[realptr][findlen0+1],slash-findlen0-1);
				tmp[slash-findlen0-1]='\0';
				printf("Only in %s: %s\n",tmp,&realname[realptr][slash+1]);
				realptr++;
			}
			break;
		}
		if(realptr==realcnt){
			while(cmpptr<cmpcnt){
				for(int i=0;i<strlen(cmpname[cmpptr]);i++){
					if(cmpname[cmpptr][i]=='/'){
						slash = i;
					}
				}
				strncpy(tmp,cmpname[cmpptr],slash);
				printf("Only in %s: %s\n",tmp,&cmpname[cmpptr][slash+1]);
				cmpptr++;
			}
			break;
		}
		int string_cmp =0;
		if(option[2]){
			string_cmp = strcasecmp(cmpname[realptr], &realname[realptr][dirgap]);
		}else{
			string_cmp = strcmp(cmpname[realptr], &realname[realptr][dirgap]);
		}

		if(string_cmp>0){ //비교하고자 하는게 더 앞에 있을시 
			for(int i=0;i<strlen(realname[realptr]);i++){
				if(realname[realptr][i]=='/'){
					slash = i;
				}
			}
			strncpy(tmp,&realname[realptr][findlen0+1],slash-findlen0-1);
			printf("Only in %s: %s\n",tmp,&realname[realptr][slash+1]);
			realptr++;
		}else if(string_cmp<0){ //비교하고자 하는게 뒤에 있을 시
			for(int i=0;i<strlen(cmpname[cmpptr]);i++){
				if(cmpname[cmpptr][i]=='/'){
					slash = i;
				}
			}
			strncpy(tmp,cmpname[cmpptr],slash);
			printf("Only in %s: %s\n",tmp,&cmpname[cmpptr][slash+1]);
			cmpptr++;
		}else{ // 같을 때
			struct stat realst;
			struct stat cmpst;
			stat(cmpname[cmpptr],&cmpst);
			stat(realname[realptr],&realst);
			if(S_ISDIR(realst.st_mode)!=S_ISDIR(cmpst.st_mode)){ //  두 파일의 형식이 다름 
				if(S_ISDIR(realst.st_mode)){
					printf("File %s is a regular file while file %s is a directory\n",cmpname[cmpptr],&realname[realptr][findlen0+1]);
				}else{
					printf("File %s is a directory while file %s is a regular file\n",cmpname[cmpptr],&realname[realptr][findlen0+1]);
				}
			}else if(S_ISDIR(realst.st_mode)){  // 둘다 디렉토리임 
				if(option[3]){ // 재귀 O                              @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
					struct stat tmpst; //strstr으로 파일이름 포함한다면 진행... 
					char *opend = &realname[realptr][findlen+1];
					int saveptrr = realptr;
					int saveptrc = cmpptr;
					realptr++;
					cmpptr++;
					int check_same=0;
					while(realptr<realcnt&&(strstr(&realname[realptr][findlen+1],opend)!=NULL)){ // 이름 안 디렉토리 탐색 
						if(cmpptr==cmpcnt){ // cmp가 범위 초과함
							check_same =1;
							break;
						}
						if(option[2]){
							string_cmp = strcasecmp(cmpname[realptr], &realname[realptr][findlen+1]);
						}else{
							string_cmp = strcmp(cmpname[realptr], &realname[realptr][findlen+1]);
						}

						if(string_cmp!=0){ // 둘의 이름이 다르다면 
							check_same =1;
							break;
						}else{ // 둘의 이름이 같다면 
							stat(cmpname[cmpptr],&cmpst);
							stat(realname[realptr],&realst);
							if(S_ISDIR(cmpst.st_mode)!=S_ISDIR(cmpst.st_mode)){ // 둘의 이름만 같고 형식 다름 
								check_same = 1;
								break;
							}else if(S_ISDIR(cmpst.st_mode)){ // 둘다 디렉토리 

							}else{
								diffcheck =FileDiff(cmpname[cmpptr],realname[realptr],1);
								if(diffcheck){ // 내용이 다름 
									check_same=1;
									break;
								}
							}
						}
						realptr++;
						cmpptr++;
					}
					if(check_same){ // 디렉토리 내부가 다름 
						realptr = saveptrr;
						cmpptr = saveptrc;
					}else{ // 디렉토리 내부가 같음 
						printf("Common subdirectories: %s and %s\n",cmpname[saveptrc-1],opend);
						continue;
					}
				}else{ // 재귀 X 
					printf("Common subdirectories: %s and %s\n",cmpname[cmpptr],&realname[realptr][findlen0+1]);
				}
			}else{ // 둘다 그냥 파일임 
				diffcheck =FileDiff(cmpname[cmpptr],realname[realptr],1);
				if(diffcheck){ // 다른내용 있음
					if(option[0]){
						printf("Files %s and %s are identical\n",cmpname[cmpptr],&realname[realptr][findlen0+1]);
					}else{ 
						printf("diff %s%s%s%s%s %s\n",option[0]?"-q ":"",option[1]?"-s ":"",option[2]?"-i ":"",option[3]?"-r ":"",cmpname[cmpptr],&realname[realptr][findlen0+1]);
						FileDiff(realname[realptr],cmpname[cmpptr],0);
					}
				}else{ // 같은 내용임
					if(option[1]){
						printf("Files %s and %s are identical\n",cmpname[cmpptr],&realname[realptr][findlen0+1]);
					}
				} 
			}
			realptr++;
			cmpptr++;
		}
	} 

}
int FileDiff(char *namex, char *name0,int x){
	struct stat realst,cmpst;
	FILE *real_file;
	FILE *cmp_file;
	char *real_line = NULL;
	ssize_t real_n;
	char *cmp_line = NULL;
	ssize_t cmp_n;
	diffcheck=0;
	char *cmp_tmp=NULL;
	char *real_tmp=NULL;
	int real_len,cmp_len;
	int real_lenn, cmp_lenn;
	int real_start=0,real_end=0,cmp_start=0,cmp_end=0;
	int real_start_num=1,real_end_num=1,cmp_start_num=1,cmp_end_num=1;
	stat(namex,&realst);
	stat(name0,&cmpst);
	int realsize = realst.st_size;
	int cmpsize = cmpst.st_size;
	if((real_file=fopen(namex,"r+"))==NULL){
		printf("file error!\n");
		return;
	}
	if((cmp_file=fopen(name0,"r+"))==NULL){
		printf("file error!\n");
		return;
	}
	int a=0;
	int check=0;
	while((real_n = getline(&real_line,&real_len,real_file))!=-1){
		cmp_end = cmp_start;
		cmp_end_num = cmp_start_num;
		fseek(cmp_file,cmp_start,SEEK_SET);
		while((cmp_n = getline(&cmp_line, &cmp_len, cmp_file))!=-1){
			if(!strcmp(real_line,cmp_line)){ //비교하는 두 라인이 같을 때
				if(((cmpsize==cmp_start+cmp_n)&&(realsize!=real_start+real_n))||((cmpsize!=cmp_start+cmp_n)&&(realsize==real_start+real_n))){
					cmp_end +=cmp_n;
					cmp_end_num++;
					continue;
				}

				if(!strcmp(real_line,"\n")){ // \n으로 같을 때 
					ssize_t cmp_nn = getline(&cmp_tmp,&cmp_lenn,cmp_file);
					ssize_t real_nn = getline(&real_tmp,&real_lenn,real_file);
					if(!strcmp(cmp_tmp,real_tmp)){ // 아래 라인이 같을 때
						fseek(cmp_file,-cmp_nn,SEEK_CUR);
						fseek(real_file,-real_nn,SEEK_CUR);
					}else{ // 아래 라인이 다를 때 
						fseek(cmp_file,-cmp_nn,SEEK_CUR);
						fseek(real_file,-real_nn,SEEK_CUR);
						cmp_end+=cmp_n;
						cmp_end_num++;
						continue;
					}
				}
				if((real_end!=real_start)&&(cmp_end!=cmp_start)){ // 변경 c
					if(x) return 1; 
					PrintSame(cmp_start_num,cmp_end_num-1);
					printf("c");
					PrintSame(real_start_num,real_end_num-1);
					printf("\n");
					fseek(cmp_file,cmp_start,SEEK_SET);
					while(cmp_end!=cmp_start){
						ssize_t cmp_nn = getline(&cmp_line,&cmp_len,cmp_file);
						printf("< %s",cmp_line);
						if(cmpsize==cmp_start+cmp_nn){
							printf("\\ No newline at end of file\n");
						}
						cmp_start+=cmp_nn;
						cmp_start_num++;
					}
					printf("---\n");
					fseek(real_file,real_start,SEEK_SET);
					while(real_end!=real_start){
						ssize_t real_nn = getline(&real_line,&real_len,real_file);
						real_start+=real_nn;
						printf("> %s",real_line);				
						if(realsize==real_start){
							printf("\\ No newline at end of file\n");
						}
						real_start_num++;
					}
				}else if(real_end!=real_start){ // 추가  a
					if(x) return 1;
					//printf("%d %d\n",real_end,real_start);
					printf("%d",cmp_start_num-1);
					printf("a");
					PrintSame(real_start_num,real_end_num-1);
					printf("\n");
					fseek(real_file,real_start,SEEK_SET);
					while(real_end!=real_start){
						ssize_t real_nn = getline(&real_line,&real_len,real_file);
						real_start+=real_nn;
						printf("> %s",real_line);
						if(realsize==real_start){
							printf("\\ No newline at end of file\n");
						}
						real_start_num++;
					}
				}else if(cmp_end!=cmp_start){ // 삭제 d
					if(x) return 1;
					PrintSame(cmp_start_num,cmp_end_num-1);
					printf("d");
					printf("%d\n",real_start_num-1);
					fseek(cmp_file,cmp_start,SEEK_SET);
					while(cmp_end!=cmp_start){
						ssize_t cmp_nn = getline(&cmp_line,&cmp_len,cmp_file);
						printf("< %s",cmp_line);
						if(cmpsize==cmp_start+cmp_nn){
							printf("\\ No newline at end of file\n");
						}
						cmp_start+=cmp_nn;
						cmp_start_num++;
					}
				}else{

				}
				cmp_end+=cmp_n;
				cmp_start+=cmp_n;
				cmp_end_num++;
				cmp_start_num++;
				real_start_num++;
				real_start+=real_n;

				fseek(real_file,real_start,SEEK_SET);
				break;
			}else{
				cmp_end+=cmp_n;
				cmp_end_num++;

			}
		}
		real_end_num++;
		real_end+=real_n;
		//printf("%d %d---\n",real_start,real_end);
	}

	if((real_end!=real_start)&&(cmp_end!=cmp_start)){ // c 
		if(x) return 1;
		PrintSame(cmp_start_num,cmp_end_num-1);
		printf("c");
		PrintSame(real_start_num,real_end_num-1);
		printf("\n");
		fseek(cmp_file,cmp_start,SEEK_SET);
		while(cmp_end!=cmp_start){
			ssize_t cmp_nn = getline(&cmp_line,&cmp_len,cmp_file);
			printf("< %s",cmp_line);
			if(cmpsize==cmp_start+cmp_nn){
				printf("\\ No newline at end of file\n");
			}
			cmp_start+=cmp_nn;
			cmp_start_num++;
		}
		printf("---\n");
		fseek(real_file,real_start,SEEK_SET);
		while(real_end!=real_start){
			ssize_t real_nn = getline(&real_line,&real_len,real_file);
			real_start+=real_nn;
			printf("> %s",real_line);
			if(realsize==real_start){
				printf("\\ No newline at end of file\n");
			}
			real_start_num++;
		}
	}else if(real_end!=real_start){
		if(x) return 1;
		//printf("%d %d\n",real_end,real_start);
		printf("%d",cmp_start_num-1);
		printf("a");
		PrintSame(real_start_num,real_end_num-1);
		printf("\n");
		fseek(real_file,real_start,SEEK_SET);
		while(real_end!=real_start){
			ssize_t real_nn = getline(&real_line,&real_len,real_file);
			real_start+=real_nn;
			printf("> %s",real_line);
			if(realsize==real_start){
				printf("\\ No newline at end of file\n");
			}
			real_start_num++;
		}
	}else if(cmp_end!=cmp_start){
		if(x) return 1;
		PrintSame(cmp_start_num,cmp_end_num-1);
		printf("d");
		printf("%d\n",real_start_num-1);
		printf("\n");
		fseek(cmp_file,cmp_start,SEEK_SET);
		while(cmp_end!=cmp_start){
			ssize_t cmp_nn = getline(&cmp_line,&cmp_len,cmp_file);
			printf("< %s",cmp_line);
			if(cmpsize==cmp_start+cmp_nn){
				printf("\\ No newline at end of file\n");
			}
			cmp_start+=cmp_nn;
			cmp_start_num++;
		}
	}
	fclose(cmp_file);
	fclose(real_file);
}

long long FindSame(char *findname, char *filename){
	struct stat st2;
	struct dirent *file;
	DIR *dir2;
	char name[300];
	long long size=0;

	if((dir2 = opendir(filename))==NULL){
		printf("error2 %d",errno);
		return -1;
	}

	while((file=readdir(dir2))!=NULL){
		if(strcmp(file->d_name,".")==0||strcmp(file->d_name,"..")==0)
			continue;
		sprintf(name,"%s/%s",filename,file->d_name);
		stat(name,&st2);
		if(S_ISDIR(st2.st_mode)){
			long long f =  FindSame(findname,name);
			size+=f;
			if((f==find_size)&&(!strcmp(file->d_name,findname))){
				PrintAll(&st2,f,name);
			}
		}else{		
			if((st2.st_size==find_size)&&(!strcmp(file->d_name,findname))){
				PrintAll(&st2,st2.st_size,name);
			}
			size+=(long long)st2.st_size;
		}
	}

	closedir(dir2);

	return size;
}
void find(char *filename, char *path){
	filenum=0;
	int index;
	findlen = strlen(filename);
	findlen0 = strlen(path);
	getcwd(cur_dir,255);
	real_find_path=realpath(filename,NULL);
	//printf("%s\n",real_find_path);
	//dir = opendir(filename);
	if(stat(filename,&st)==-1){
		printf("error!,%d",errno);
		return;
	}else{
		//printf("%lo %lld",S_ISDIR(st.st_mode),(long long)st.st_size);
		if(S_ISDIR(st.st_mode)){
			find_size = SizeSum(filename);
		}else{
			find_size = (long long)st.st_size;
		}

	}

	printf("Index Size  Mode       Blocks Links UID  GID  Access         Change         Modify         Path                     \n");
	PrintAll(&st,find_size,filename);
	FindSame(filename,path);
	if(filenum==1){
		printf("(None)\n");
	}else{
		while(1){
			int check=1;
			printf(">> ");
			scanf("%d",&index);
			if(S_ISDIR(st.st_mode)){
				gets(buf,10,stdin);
				for(int i=0;i<4;i++){
					option[i]=0;}
				for(int i=0;i<strlen(buf);i++){
					if(buf[i]==' '){
						continue;
					}else if(buf[i]=='q'&&(buf[i+1]<'a'||buf[i+1]>'z')){
						option[0]=1;
					}else if(buf[i]=='i'&&(buf[i+1]<'a'||buf[i+1]>'z')){
						option[1]=1;
					}else if(buf[i]=='s'&&(buf[i+1]<'a'||buf[i+1]>'z')){
						option[2]=1;

					}else if(buf[i]=='r'&&(buf[i+1]<'a'||buf[i+1]>'z')){
						option[3]=1;
					}else{
						for(int j=0;j<4;j++){
							option[j]=0;
						}
						printf("input error\n");
						check=0;
						break;
					}
				}
				if(check){
					DirDiff(index);
					break;
				}else{
					continue;
				}	
			}
			else{
				FileDiff(name_list[index],name_list[0],0);
				break;
			}
		}
	}
}
