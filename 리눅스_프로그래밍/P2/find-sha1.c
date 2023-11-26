#include "ssu_sdup.h"

typedef struct set{
	int file_num;
	unsigned char hashnum[20];
	int size;
	struct set* ptr;
	setnode* head;	
	setnode* tail;
}set;
int check_ = 0;
char queue[QUEUE_MAX][4097];
int file_max;
int file_min;
int runcnt;
struct tm *time_info;
set* set_head;
set* set_tail;
int front;
int rear;
unsigned char data[1024*16];
char file_extension[10];
char buf[4096];
setnode *sn;
setnode *snn;
void PrintTime(struct stat *st){
	struct tm *t;
	t = gmtime(&st->st_mtime);
	printf("(mtime : %02d-%02d-%02d %02d:%02d:%02d) ",t->tm_year-100,t->tm_mon,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	t = gmtime(&st->st_atime);
	printf("(atime : %02d-%02d-%02d %02d:%02d:%02d)\n",t->tm_year-100,t->tm_mon,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}

int Full(void){
	int tmp = (rear+1)%QUEUE_MAX;
	if(tmp == front){
		return 1;
	}
	else
		return 0;
}

void push(char *name){
	if(Full()){
		printf("Queue is full\n");
	}
	else{
		rear = (rear+1)%QUEUE_MAX;
		strcpy(queue[rear],name);
	}
}

void MakeBound(int a,char* num){
	if(num[0]=='~'){
		if(strlen(num)!=1){ // 예외 문자열 입력됨
			return;
		}else{ // ~ 입력됨 
			if(a==0){ // Min
				file_min = 0;
			}else{ // max
				file_max = 9999999;
			}
			return;
		}
	}

	int ansnum=0;
	for(int i=0;i<strlen(num);i++){ // ~ 제외 나머지 처리 
		if(!(num[i]>='A'&&num[i]<='z')&&!(num[i]>='0'&&num[i]<='9')){ // 다른 문자 입력됨
			if(num[i]!='.')
				return;
		}
		if(num[i]>='A'&&num[i]<='z'){
			if(strcasecmp(&num[i], "MB")==0){ // 
				double d = atof(num);
				ansnum =(int)(d*1000000);
			}else if(strcasecmp(&num[i], "GB")==0){
				double d = atof(num);
				ansnum =(int)(d*1000000000);
			}else if(strcasecmp(&num[i], "KB")==0){
				double d = atof(num);
				ansnum =(int)(d*1000);
			}else{ // 입력 실패
				return;
			}
			if(a==0)//Min
				file_min = ansnum;
			else
				file_max = ansnum;
			return;
		}
	}
	double d = atof(num); // 숫자만 입력 들어옴 
	ansnum = (int)d;
	if(a==0)
		file_min = ansnum;
	else
		file_max = ansnum;
	return;
}
setnode *s;
setnode* MakeSetnode(char *n){
	s = (setnode *)calloc(1,sizeof(setnode));
	char *nam = (char *)malloc(strlen(n)+1);

	strcpy(nam,n);
	s->name= nam;
	s->ptr = NULL;
	return s;
}
void FileDelete(set *st, setnode *stfirst, setnode *stsecond){ // 파일 지우는 함수 

	if (remove(stsecond->name)==-1){ // 파일 지우기
		fprintf(stderr, "file remove error");
		exit(1);
	}
	if(st->tail==stsecond){ //가리키는게 제일 끝 파일
		st->tail = stfirst;
		st->tail->ptr =NULL;
		sn = NULL;
		free(stsecond);
	}else if(st->head == stsecond){ // 맨 앞 파일 지우기 
		st->head = st->head->ptr;
		snn = st->head;
		free(stsecond);
		sn = snn;
	}else{ // 중간 파일 지우기 
		stfirst->ptr = stsecond->ptr;
		free(stsecond);
		sn = snn->ptr;
	}
	st->file_num--;
}

void PrintAll(){
	set* st = set_head;
	printf("\n");
	while(st!=NULL){
		setnode *stn = st->head;
		printf("%d-> ",st->size);
		while(stn!=NULL){
			printf("%s| ",stn->name);
			stn = stn->ptr;
		}
		printf("\n");
		st=st->ptr;
	}
	printf("\n");
}
void RemoveList(){
	set* setptr = set_head;
	set* setptr2 =set_head;

	while(setptr!=NULL){
		setptr2 = setptr->ptr;
		setnode* setn=setptr->head;
		setnode* setn2=setptr->head;
		while(setn!=NULL){
			setn2 = setn->ptr;
			free(setn);
			setn=setn2;
		}
		free(setptr);
		setptr =setptr2;
	}
	set_head=NULL;
	set_tail = NULL;
}
void file_init(){

	file_max=-1;
	file_min=-1;
	runcnt=0;
	set_head=NULL;
	set_tail = NULL;
	front = -1;
	rear = -1;
}
char nametmp[4095];
void fsha1(char *File_ex, char* Min, char* Max , char* Target){
	file_init();
	file_extension[0]='\0'; // 확장자명 초기화 
	MakeBound(0,Min); // min값 변환, 저장
	MakeBound(1,Max); // max값 변환, 저장
	if(Target[0]=='~'){
		sprintf(nametmp, "/home/jh/%s%c",&Target[2],'\0');
	}else{
		strcpy(nametmp, Target);
	}

	if(file_max==-1||file_min==-1){ // 올바르지 않은 input 거름 
		printf("wrong input\n");
		return;
	}

	if(File_ex[0]!='*'||((strlen(File_ex)!=1)&&(File_ex[1]!='.'))){ // 파일 확장자 처리 
		printf("wrong input\n");
		return;
	}
	if(strlen(File_ex)!=1){ // 확장자기 있을 경우 
		for(int i=2;i<strlen(File_ex);i++){
			if(File_ex[i]<'a'||File_ex[i]>'z'){
				printf("wrong input\n");
				return;
			}
		}
		strcpy(file_extension,&File_ex[1]);
	}
	push(nametmp);
	DIR *dir=NULL;
	struct dirent *file = NULL;
	char tmpname[4096];
	struct stat st;
	FILE* fp;
	unsigned char fshatmp[20];
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	while(front!=rear){ // 디렉토리면 탐색, 큐 추가하고, 파일이면 해쉬 비교후 리스트 추가한다.
		if((dir = opendir(queue[(front+1)%QUEUE_MAX]))==NULL){ // 디렉토리 오픈 
			fprintf(stderr, "directory name error\n");
			exit(1);
		}
		while((file = readdir(dir))!=NULL){ // 디렉토리 탐색
			if(strcmp(file->d_name,".")==0||strcmp(file->d_name, "..")==0){
				continue;
			}
			if(strcmp(queue[(front+1)%QUEUE_MAX],"/")==0){

				sprintf(tmpname, "/%s",file->d_name); //tmpname에다 파일이름 만들기 
			}else{
				sprintf(tmpname, "%s/%s", queue[(front+1)%QUEUE_MAX],file->d_name); //tmpname에다 파일이름 만들기 
			}
			if(stat(tmpname,&st)==-1){
				printf("file stat error");
				exit(1);
			}

			if(!strcmp(tmpname,"/proc")||!strcmp(tmpname,"/run")||!strcmp(tmpname,"/sys")){
				continue;
			}

			if(S_ISDIR(st.st_mode)){ // 디렉토리라면

				push(tmpname);
			}else if(S_ISREG(st.st_mode)){ // 일반 파일이라면 

				if(st.st_size<file_min||st.st_size>file_max){ // 조건 확인

					continue;
				}
				if((strlen(file_extension)!=0)&&(strstr(tmpname,file_extension)==NULL)){

					continue;
				}

				memset(data, 0, 4096); // 초기화
				/*MD5_CTX ctx; // md5 해시 만들기 
				if((fp =fopen(tmpname, "rb"))==NULL){
					printf("file open error\n");
					break;
				}

				MD5_Init(&ctx);
				int bytes;
				while((bytes = fread(data, 1, 4096,fp))!=0)
					MD5_Update(&ctx, data, bytes);
				MD5_Final(fshatmp, &ctx); */
				//@@@@@@@@@@@@@
				//@@@@@@@@@@@@@@@@@
				
				SHA_CTX ctx; // fsha 해시 만들기 
				if((fp =fopen(tmpname, "rb"))==NULL){
					printf("file open error\n");
					break;
				}

				SHA1_Init(&ctx);
				int bytes;
				while((bytes = fread(data, 1, 4096,fp))!=0)
					SHA1_Update(&ctx, data, bytes);
				SHA1_Final(fshatmp, &ctx);
				
				/*
				   printf("%s ", file->d_name);
				   for(int i=0;i<20;i++){
				   printf("%0X",fshatmp[i]);
				   }
				   printf("\n");
				 */
				set* setptr=set_head;
				while(setptr!=NULL){ // set 링스크리스트 탐색 
					if(setptr->size ==st.st_size){ // 파일 사이즈 같음 
						int tx;
						tx=strcmp(setptr->hashnum,fshatmp);
						
						if((tx=strcmp(setptr->hashnum,fshatmp))==0){ // 해시값도 같음
							setptr->file_num +=1;
							sn =MakeSetnode(tmpname);
							setptr->tail->ptr = sn;
							setptr->tail = sn;
							break;
						}
					}
					setptr = setptr->ptr; // 다음 set으로 넘어감 
					//printf("%d\n", setptr);
				}

				if(setptr == NULL){ //같은 해시값이 없음 
					set *s = (set *)malloc(sizeof(set)); // 새로운 set 만들기, 값 대입
					s->file_num=1;
					s->ptr = NULL;
					strcpy(s->hashnum, fshatmp);
					s->size = st.st_size;
					sn = MakeSetnode(tmpname);
					s->head = sn;
					s->tail = sn;
					s->head->ptr = NULL;
					if(set_tail == NULL){ // set에 아무것도 들어있지 않음 
						set_head = s;
						set_tail = s;
						set_tail->ptr=NULL;
					}else{ //set에 무언가 있음 
						set *frontset= set_head;
						set *backset = set_head;
						while(backset!=NULL){ // 크기순 위치 찾기 
							if(backset->size<s->size){ // 현재 포인터 크기보다 더 크다면 
								frontset = backset;
								backset = backset->ptr;
							}else{
								break;
							}
						}
						if(frontset==backset){ // 맨 앞에 넣어야함
							s->ptr = set_head;
							set_head = s;

						}else if(backset==NULL){ // 맨 뒤에 넣어야함 
							set_tail->ptr = s;
							set_tail = s;
							set_tail->ptr=NULL;
						}else{ // 가운데 넣어야함
							s->ptr = backset;
							frontset->ptr = s;
						}
					}	
				}
				fclose(fp);
			}
		}
		closedir(dir);
		front=(front+1)%QUEUE_MAX; // queue pop
	}
	gettimeofday(&end, NULL);
	// 링크드 리스트 만들기 끝
	int timeprint=0;

	while(1){

		set* stmp = set_head;
		set* sstmp=set_head;
		int setcnt = 0;
		while(stmp!=NULL){ // set 순환 , 출력
			if(stmp->file_num>1){ // 같은 해시 가진게 2개 이상이라면 
				printf("---- Identical files #%d (%d bytes - ",++setcnt,stmp->size);
				for(int i=0;i<20;i++){
					printf("%02x",stmp->hashnum[i]);
				}
				printf(") ----\n");
				sn = stmp->head;
				int setnodecnt=0;
				while(sn!=NULL){ // setnode 순환
					if(realpath(sn->name,buf)==NULL){
						printf("real path error\n");
						exit(1);
					}

					printf("[%d] %s ",++setnodecnt,buf);
					stat(buf,&st);
					PrintTime(&st);
					sn = sn->ptr;
				}
				printf("\n");
			}else{ // 같은 해시 하나밖에 없음
				if((sstmp==stmp)&&(stmp->ptr==NULL)){ // 안에 파일 하나밖에 없음 
					sn = stmp->head;
					free(sn);
					free(stmp);
					set_head = NULL;
					set_tail = NULL;
				}else if(stmp==set_tail){ // 파일 두개 이상 있음
					// 맨 뒤 파일 지우기
					sn = stmp->head; 
					set_tail = sstmp;
					set_tail->ptr = NULL;
					sn = stmp->head;
					free(sn);
					free(stmp);

				}else if(stmp == sstmp){ // 맨 앞 파일 지우기
					sn = stmp->head;
					set_head = set_head->ptr;
					sn = stmp->head;
					free(sn);
					free(stmp);
					stmp = set_head;
					sstmp = set_head;
					continue;
				}else{ // 중간 파일 지우기 
					sstmp->ptr = stmp->ptr;
					sn = stmp->head;
					sn = stmp->head;
					free(sn);
					free(stmp);
				}

				stmp = stmp->ptr;
				continue;
			}
			sstmp = stmp;
			stmp = stmp->ptr;
		}
		if(setcnt==0){ //같은 파일 없을때 메세지
			realpath(Target,buf);
			printf("No duplicates in %s\n",buf);
		}
		if(timeprint==0){
			timeprint++;
			printf("Searching time: "); // 서칭 시간 출력
			long gap_sec = end.tv_sec-start.tv_sec;
			long gap_usec = end.tv_usec-start.tv_usec;
			if(gap_usec<0){
				gap_sec-=1;
				gap_usec+=1000000;
			}
			printf("%ld:%06ld(sec:usec)\n\n",gap_sec, gap_usec);
		}
		if(setcnt==0){
			return;
		}
		while(1){
			if(setcnt!=0){ // 지우는 옵션구현
				printf(">>");
				char option[10];
				char *ptrset[3]={NULL,NULL,NULL};
				int cn =0;
				fgets(option, 10, stdin);
				for(int i=0;i<10;i++){
					if(option[i]=='\n'){
						option[i]='\0';
					}
				}
				char *opptr = strtok(option, " ");
				while(opptr!=NULL){
					ptrset[cn++]=opptr;
					opptr = strtok(NULL," ");
				}

				if(strcmp(ptrset[0],"exit")==0){
					printf(">> Back to Prompt\n");

					RemoveList();
					return;
					if(ptrset[0]==NULL||ptrset[1]==NULL)
						continue;
				}else{
					int selectnum = atoi(ptrset[0]);
					if(selectnum>setcnt){
						printf("wrong number\n");
						break;
					}else if(strcmp(ptrset[1],"d")==0){ // d 옵션 
						if(ptrset[2]==NULL){ // 2번쨰 옵션 없음 
							printf("input error\n");
							break;
						}
						else{
							int nodenum=atoi(ptrset[2]);
							set *stmp = set_head;

							for(int s=1;s<selectnum;s++){
								stmp = stmp->ptr;
							}

							if(nodenum>stmp->file_num){ // 예외처리
								printf("wrong number\n");
								break;
							}
							sn = stmp->head;
							snn = stmp->head;
							for(int s=1;s<nodenum;s++){
								if(s!=1){
									snn=snn->ptr;
								}
								sn = sn->ptr;

							}
							// 위치 이동 끝
							FileDelete(stmp, snn,sn);
							realpath(sn->name, buf);
							printf("\"%s\" has been deleted in #%d\n\n",buf,selectnum);
							break;
						}
					}else if(strcmp(ptrset[1],"i")==0){ // i 옵션
						set *stmp = set_head;

						for(int s=1;s<selectnum;s++){
							stmp = stmp->ptr;
						}
						sn = stmp->head;
						snn = stmp->head;
						char yn[3];
						while(sn!=NULL){
							realpath(sn->name,buf);
							printf("Delete \"%s\"? [y/n] ",buf);
							fgets(yn,3,stdin);
							if(yn[0]=='y'||yn[0]=='Y'){ // 지운다.
								if(stmp->file_num==1){
									RemoveList();
									printf("\n");
									return;
								}
								FileDelete(stmp, snn,sn);

								continue;
							}else if(yn[0]=='N'||yn[0]=='n'){ // 안지운다

							}else{//에러 
								printf("input error\n");
								break;
							}
							if(snn!=sn){
								snn=snn->ptr;
							}
							sn = sn->ptr;

						}
						printf("\n");

						break;
					}else if(strcmp(ptrset[1],"f")==0||strcmp(ptrset[1],"t")==0){ // f옵션 
						set *stmp = set_head;

						for(int s=1;s<selectnum;s++){ // 현재 set 위치 찾기 
							stmp = stmp->ptr;
						}
						setnode *s1=stmp->head; // setnode 처음부터 시작 
						stmp->tail->ptr = NULL;
						int tlist[6]={0,0,0,0,0,0};
						setnode *recent=NULL;
						struct stat stt;
						while(s1!=NULL){ // 제일 큰 파일 찾기
							if(stat(s1->name, &stt)==-1)
								printf("stat error\n");

							struct tm *tt = gmtime(&stt.st_mtime);
							for(int i=0;i<6;i++){ // 시간별 비교 
								if(i==0){
									if(tt->tm_year<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_year==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신

										recent = s1;
										tlist[i]=tt->tm_year;
									}
								}else if(i==1){
									if(tt->tm_mon<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_mon==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신
										recent = s1;
										tlist[i]=tt->tm_mon;
									}
								}else if(i==2){
									if(tt->tm_mday<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_mday==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신
										recent = s1;
										tlist[i]=tt->tm_mday;
									}
								}else if(i==3){
									if(tt->tm_hour<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_hour==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신
										recent = s1;
										tlist[i]=tt->tm_hour;
									}
								}else if(i==4){
									if(tt->tm_min<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_min==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신
										recent = s1;
										tlist[i]=tt->tm_min;
									}
								}else{
									if(tt->tm_sec<tlist[i]){ // 작을때 -> 종료 
										break;
									}else if(tt->tm_sec==tlist[i]){ // 같을때 -> 뒤에 계속 비교
										continue;
									}else{ // 클때 -> 갱신
										recent = s1;
										tlist[i]=tt->tm_sec;
									}
								}			
							}
							s1 = s1->ptr;
						}

						s1= stmp->head; // setnode 처음부터 시작 
						setnode *s2;
						if(strcmp(ptrset[1],"f")==0){ // f 옵션일때 

							while(s1!=NULL){ // recent 파일 제외 나머지 제거
								s2 = s1->ptr;
								if(s1!=recent){
									remove(s1->name);

									free(s1);
								}
								s1 =s2;
							}
							recent->ptr = NULL;
							stmp->head = recent;
							stmp->tail = recent;
							stmp->file_num = 1;
							realpath(recent->name,buf); // 출력 
							stat(recent->name,&stt);
							struct tm *tt = gmtime(&stt.st_mtime);
							printf("Left file in #%s : %s ",ptrset[0],buf);
							printf("(%02d-%02d-%02d %02d:%02d:%02d)\n\n",tt->tm_year-100,tt->tm_mon,tt->tm_mday,tt->tm_hour,tt->tm_min,tt->tm_sec);
							break;
						}else if(strcmp(ptrset[1],"t")==0){ //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
							char *trash ="/home/jh/.local/share/Trash/files";
							while(s1!=NULL){ // recent 파일 제외 나머지 제거
								s2 = s1->ptr; 
								if(s1!=recent){ // recent 파일이 아닐 때 
									int pos=0;
									for(int k=0;k<strlen(s1->name);k++){
										if(s1->name[k]=='/'){
											pos = k;
										}
									}
									sprintf(tmpname, "%s/%s",trash,&s1->name[pos+1]);
									rename(s1->name,tmpname);
									free(s1);
								}
								s1 =s2;
							}
							stmp->file_num=1;
							stmp->head = recent;
							stmp->tail = recent;
							realpath(recent->name,buf); // 출력 
							stat(recent->name,&stt);
							struct tm *tt = gmtime(&stt.st_mtime);

							printf("All files in #%s have moved to trash except \"%s\" ",ptrset[0],buf);
							printf("(%02d-%02d-%02d %02d:%02d:%02d)\n\n",tt->tm_year-100,tt->tm_mon,tt->tm_mday,tt->tm_hour,tt->tm_min,tt->tm_sec);
							break;

						}
					}else{
						printf("wrong input\n");
						break;
					}
				}
			}else{ // 지울 수 있는 파일 없을때
				break;
			}
		}
	}
	RemoveList();
}

int main(int argc, char* argv[]){
	fsha1(argv[0],argv[1],argv[2],argv[3]);

}
