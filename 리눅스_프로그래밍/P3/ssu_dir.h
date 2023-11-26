#ifndef __DIR__
#define __DIR__

#define QUEUE_SIZE 1000

extern char extension[ARGMAX]; // 확장자 저장 
extern long long minbsize, maxbsize; // 사이즈 범위 저장 
extern char target[PATHMAX]; // 목표 디렉토리 경로 
extern int thread_num;
char queue[QUEUE_SIZE][PATH_MAX];
fileList* dups_list_h;

int front=-1;
int back=-1;

char *get_extension(char *filename)
{
	char *tmp_ext;

	if ((tmp_ext = strstr(filename, ".tar")) != NULL || (tmp_ext = strrchr(filename, '.')) != NULL)
		return tmp_ext + 1;
	else
		return NULL;
}
void fileinfo_append(fileInfo *head, char *path) // 새로운 파일info 추가 함수 (넣을 list 위치, 경로)
{
	fileInfo *fileinfo_cur; //새로운 fileinfo 만들기 

	fileInfo *newinfo = (fileInfo *)malloc(sizeof(fileInfo));
	memset(newinfo, 0, sizeof(fileInfo)); // 초기화 
	strcpy(newinfo->path, path); // path 할당 
	lstat(newinfo->path, &newinfo->statbuf); // lstat 할당 
	newinfo->next = NULL; 

	if (head->next == NULL) // 안에 비었으면 
		head->next = newinfo; 
	else { //다른 파일 존재시 
		fileinfo_cur = head;
		while (fileinfo_cur->next != NULL)
			fileinfo_cur = fileinfo_cur->next;

		fileinfo_cur->next = newinfo; // 맨 끝에 붙여줌 
	}

}
void filelist_append( long long filesize, char *path, char *hash) //새로운 파일리스트 추가 함수 
{
	fileList *newfile = (fileList *)malloc(sizeof(fileList)); // 새 파일리스트 생성 
	memset(newfile, 0, sizeof(fileList)); // 초기화 
	newfile->filesize = filesize; // 크기 할당 
	strcpy(newfile->hash, hash);//해시 할당 
	newfile->filenum = 1; // 안의 리스트 크기 할당 
	newfile->fileInfoList = (fileInfo *)malloc(sizeof(fileInfo)); // 새로운 리스트 생성 
	memset(newfile->fileInfoList, 0, sizeof(fileInfo)); // 초기화 
	fileinfo_append(newfile->fileInfoList, path); // 경로 할당 
	newfile->next = NULL;

	if (dups_list_h->next == NULL) { // 리스트 하나도 없을 때 
		dups_list_h->next = newfile;
	}    
	else { //리스트 파일 존재할 시 
		fileList *cur_node = dups_list_h->next, *prev_node = dups_list_h, *next_node;
		while (cur_node != NULL && cur_node->filesize < newfile->filesize) { // 크기에 맞게 넣어줌 
			prev_node = cur_node;
			cur_node = cur_node->next;
		}

		newfile->next = cur_node;
		prev_node->next = newfile;
	}    
}
int md5(char *target_path, char *hash_result)
{
	FILE *fp;
	unsigned char hash[MD5_DIGEST_LENGTH];
	unsigned char buffer[SHRT_MAX];
	int bytes = 0;
	MD5_CTX md5;

	if ((fp = fopen(target_path, "rb")) == NULL){
		printf("ERROR: fopen error for %s\n", target_path);
		return 1;
	}

	MD5_Init(&md5);

	while ((bytes = fread(buffer, 1, SHRT_MAX, fp)) != 0)
		MD5_Update(&md5, buffer, bytes);

	MD5_Final(hash, &md5);

	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(hash_result + (i * 2), "%02x", hash[i]);
	hash_result[HASHMAX-1] = 0;

	fclose(fp);

	return 0;
}

void get_fullpath(char *target_dir, char *target_file, char *fullpath) //디렉토리 경로, 안의 파일 이름 받아서 전체경로 만들기 
{
	strcat(fullpath, target_dir); // 앞의 디렉토리 경로 넣어줌 

	if(fullpath[strlen(target_dir) - 1] != '/') // 맨 끝이 / 가 아니면 넣어줌
		strcat(fullpath, "/");

	strcat(fullpath, target_file); // 파일 경로 넣어줌 
	fullpath[strlen(fullpath)] = '\0'; // 맨 끝 널문자 
}
void push(char* dir){ // 큐에 push
	back = (back+1)%QUEUE_SIZE;
	if(back==front){
		fprintf(stderr, "queue is full");
		exit(1);
	}
	strcpy(queue[back], dir);
}

void add_list(char *hash, char* filepath, long long filesize){
	fileList *movelist = dups_list_h->next; //파일리스트 순회할 포인터 
	while(movelist!=NULL){ // 리스트 끝까지 돌면서 같은 해쉬 찾음 
		if(!strcmp(hash, movelist->hash)){
			movelist->filenum++;
			fileinfo_append(movelist->fileInfoList, filepath); //size, path, hash
		}
		movelist = movelist->next;
	}
	if(movelist==NULL){ //같은 해쉬 없어서 새로운 리스트 추가 
		filelist_append(filesize, filepath, hash);
	}

}
void printList(){
	fileInfo *i;
	fileList *l = dups_list_h->next;
	while(l!=NULL){
		i = l->fileInfoList->next;
		printf("%s\n", l->hash);
		while(i!=NULL){
			printf("%s\n", i->path);
			i = i->next;
		}
		l = l->next;
	}
}

int make_list(){
	front = -1; // 큐 위치 
	back = -1;
	push(target);
	dups_list_h = (fileList *)malloc(sizeof(fileList));
	dups_list_h->next = NULL;
	DIR *dirp; // 디렉토리 포인터 
	struct dirent *dentry;
	struct stat statbuf;
	
	while(front!=back){ // 큐 빌때까지 순환 
		if((dirp = opendir(queue[(front+1)%QUEUE_SIZE]))==NULL){ // 디렉토리 오픈 
			fprintf(stderr, "dir open error");
			exit(1);
		}

		while((dentry = readdir(dirp))!=NULL){ // 디렉토리 안 파일 순회 
			char fullpath[PATHMAX] = {0, }; // 안의 파일 절대경로 저장 
			if (!strcmp(dentry->d_name, ".") || !strcmp(dentry->d_name, "..")) // 현위치 디렉토리, 전 디렉토리 건너뛰기 
				continue;
			get_fullpath(queue[(front+1)%QUEUE_SIZE], dentry->d_name, fullpath); // 절대경로 저장 
			if (!strcmp(fullpath,"/proc") || !strcmp(fullpath, "/run") || !strcmp(fullpath, "/sys") || !strcmp(fullpath, "/sys")) // 건너뛸 파일들 스킵 
				continue;
			if (lstat(fullpath, &statbuf) < 0){ // stat 가져오기 
				printf("ERROR: lstat error for %s\n", fullpath);
				return 0;
			}
			if((long long)statbuf.st_size<minbsize) // 사이즈 검사 
				continue;
			if(maxbsize!=-1 && (long long)statbuf.st_size>maxbsize)
				continue;

			if(S_ISDIR(statbuf.st_mode)){ // 디렉토리라면 
				push(fullpath);
			}else if(S_ISREG(statbuf.st_mode)){ // 일반 파일이라면 
				//printf("%s\n", fullpath);
				FILE *fp;
				char filename[PATHMAX*2];
				char *path_extension; // 확장자 저장 
				char hash[HASHMAX]; // 해시값 저장 
				memset(hash, 0, HASHMAX); // 헤시 초기화
				md5(fullpath, hash); // 헤시 할당 
				path_extension = get_extension(fullpath); // 확장자 가져오기 
				if (strlen(extension) != 0){ // 확장자 처리 
					if (path_extension == NULL || strcmp(extension, path_extension))
						continue;
				}
				//printf("%s %s %d\n", hash, fullpath, (long long) statbuf.st_size);
				add_list(hash, fullpath,(long long)statbuf.st_size);

			}


		}
		closedir(dirp);

		front = (front+1)%QUEUE_SIZE;
	}
	
}
#endif
