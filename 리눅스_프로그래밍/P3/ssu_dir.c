
#include "ssu_sfinder.h"

#define QUEUE_SIZE 10000

extern char extension[ARGMAX]; // 확장자 저장 
extern long long minbsize, maxbsize; // 사이즈 범위 저장 
extern char target[PATHMAX]; // 목표 디렉토리 경로 
extern int thread_num;
extern int list_type;
extern int category;
extern int order;
char same_size_files_dir[PATHMAX];

fileList* dups_list_h;
extern fileInfo *trash_list;
void print_trash_list();
fileInfo *fileinfo_delete_node(fileInfo *head, char *path);

void restore(int num){
	int size= fileinfolist_size(trash_list);
	char filename[STRMAX];
	char hash[HASHMAX];
	/*
	if(num<1||num>size){
		printf("size error\n");
		return;
	}
	fileInfo *move = trash_list->next;
	for(int i=0;i<size-1;i++){
	move = move->next;
	}
	
	fileInfo *fileinfo_delete_node(trash_list, move->path); // 쓰레기통에서 제거 
	sprintf(filename, "%s/%d", same_size_files_dir,move->statbuf.st_size);
	if ((fp = fopen(filename, "r")) == NULL){ //오픈 
			printf("ERROR: fopen error for %s\n", filename);
			continue;
		}

		while (fgets(line, sizeof(line), fp) != NULL){
			int idx;

			strncpy(hash, line, HASHMAX); // 각각 불러오기 
			hash[HASHMAX-1] = '\0';

			strcpy(filepath, line+HASHMAX);
			filepath[strlen(filepath)-1] = '\0';

			if ((idx = filelist_search(dups_list_h, hash)) == 0) // 해시 값 같으면 
				filelist_append(filesize, filepath, hash); // 그 곳에 붙어넣기 
			else{ // 같은 해시값 없음 
				fileList *filelist_cur = dups_list_h;
				while (idx--){
					filelist_cur = filelist_cur->next;
				}
				fileinfo_append(filelist_cur->fileInfoList, filepath); // 맨 끝에 넣어줌 
			}
		}

	fclose(fp);
	move = trash_list->next;
	fileList *find = dups_list_h->next;
	
	while(move!=NULL){
		
		move = move->next;
	}*/
	print_trash_list();
	
}
fileInfo *fileinfo_delete_node(fileInfo *head, char *path) // 해당하는 path 지운다 
{
	fileInfo *deleted;

	if (!strcmp(head->next->path, path)){ // 맨 앞 삭제라면 
		deleted = head->next;
		
		head->next = head->next->next;
		return head->next;
	}
	else { 
		fileInfo *fileinfo_cur = head->next;

		while (fileinfo_cur->next != NULL){ //맨 끝까지 탐색 
			if (!strcmp(fileinfo_cur->next->path, path)){ // 같은 path라면 
				deleted = fileinfo_cur->next;
				
				fileinfo_cur->next = fileinfo_cur->next->next;
				return fileinfo_cur->next;
			}

			fileinfo_cur = fileinfo_cur->next;
		}
	}
}
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
	
	newinfo->trash_time= time(NULL);
	 
	strcpy(newinfo->path, path); // path 할당 
	if(lstat(newinfo->path, &newinfo->statbuf)<0)
		printf("lstat error");
		 // lstat 할당 
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
void sort_trash(int cate, int order){
	
	 
	
	fileInfo* startinfo = trash_list; // 리스트 시작점 
		
	fileInfo *newlist = (fileInfo *)malloc(sizeof(fileInfo)); // 새롭게 만들 리스트 
	newlist -> next  = startinfo -> next; // 하나 넣어두기 
	fileInfo *next_node = startinfo->next->next->next, *cur_node =startinfo->next->next; // cur_node를 끼워넣기
	newlist->next->next = NULL;
		
		
		
		while(1){
			fileInfo *cur = newlist->next, *prev = newlist; // start 순차적으로 돌면서 맞는 위치 넣기 
			cur_node->next = NULL;
			if(cate == 0){ // filename 
				if(order==1){ // 오름차순 
					printf("1");
				while(cur != NULL && strcmp(cur->path , cur_node->path)>0){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&strcmp(cur->path , cur_node->path)<0){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 1){ // size
				if(order==1){ // 오름차순 
			
				while(cur != NULL && (cur->statbuf.st_size <cur_node->statbuf.st_size )){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&(cur->statbuf.st_size >cur_node->statbuf.st_size )){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 2){ // date
				if(order==1){ // 오름차순 
			
				while(cur != NULL && (cur->trash_time <cur_node->trash_time )){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&(cur->trash_time >cur_node->trash_time )){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 3){ // time 
				struct tm *p1,*p2;
				
				if(order==1){ // 오름차순 
			
				while(cur != NULL){
					p1 = localtime(&cur->trash_time);
					p2 = localtime(&cur_node -> trash_time); // 껴넣을거 
					if(p1->tm_hour>p2->tm_hour){
						break;
					}else if(p1->tm_hour==p2->tm_hour){
						if(p1->tm_min>p2->tm_min)
							break;
						else if(p1->tm_min==p2->tm_min){
						if(p1->tm_sec>=p2->tm_sec)
							break;	
						}
					}
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur; //끼워넣기 
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL){
					p1 = localtime(&cur->trash_time);
					p2 = localtime(&cur_node -> trash_time); // 껴넣을거 
					if(p1->tm_hour<p2->tm_hour){
						break;
					}else if(p1->tm_hour==p2->tm_hour){
						if(p1->tm_min<p2->tm_min)
							break;
						else if(p1->tm_min==p2->tm_min){
						if(p1->tm_sec<=p2->tm_sec)
							break;	
						}
					}
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur; //끼워넣기 
        			prev->next = cur_node;
				}
			}
			
			
			cur_node = next_node;
			if(next_node == NULL)
				break;
			next_node = next_node->next;
		}
		
	
	
	
}
void sort_list(int cate, int order){
	fileList *startlist = dups_list_h->next; // 시작하는 세트 
	while(startlist!=NULL){
		fileInfo *startinfo = startlist->fileInfoList; // 리스트 시작점 
		
		fileInfo *newlist = (fileInfo *)malloc(sizeof(fileInfo)); // 새롭게 만들 리스트 
		newlist -> next  = startinfo -> next; // 하나 넣어두기 
		fileInfo *next_node = startinfo->next->next->next, *cur_node =startinfo->next->next; // cur_node를 끼워넣기
		newlist->next->next = NULL;
		
		
		
		while(1){
			fileInfo *cur = newlist->next, *prev = newlist; // start 순차적으로 돌면서 맞는 위치 넣기 
			cur_node->next = NULL;
			if(cate == 0){ // filename 
				if(order==1){ // 오름차순 
			
				while(cur != NULL && strcmp(cur->path , cur_node->path)<0){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&strcmp(cur->path , cur_node->path)>0){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 1){ // size
				break;
			}else if(cate == 2){ // uid
				if(order==1){ // 오름차순 
			
				while(cur != NULL && (cur->statbuf.st_uid <cur_node->statbuf.st_uid)){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&(cur->statbuf.st_uid <cur_node->statbuf.st_uid)){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 3){ // gid 
				if(order==1){ // 오름차순 
			
				while(cur != NULL && (cur->statbuf.st_gid <cur_node->statbuf.st_gid)<0){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&(cur->statbuf.st_gid <cur_node->statbuf.st_gid)){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}else if(cate == 4){ // mode
				if(order==1){ // 오름차순 
			
				while(cur != NULL && (cur->statbuf.st_mode <cur_node->statbuf.st_mode)){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}else{ // 내림차순 
				while(cur != NULL &&(cur->statbuf.st_mode > cur_node->statbuf.st_mode)){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
				}
			}
			
			
			cur_node = next_node;
			if(next_node == NULL)
				break;
			next_node = next_node->next;
		}
		startlist->fileInfoList = newlist;
		startlist = startlist->next;
	}
	
	
}

void sort_set(int cate, int order){
	printf("%d %d", cate, order);
	int size = filelist_size(dups_list_h)-1; // 하나는 미리 붙여둠 
	if(size ==1)
		return;
	if(cate!=1){ // 크기순 말고는 정렬 안됨 
		printf("sorting failed\n");
		return;
	}else{ // 크기순 정렬
		fileList *start = (fileList *)malloc(sizeof(fileList));
		start->next = dups_list_h->next; // 맨 앞 하나 붙여둠
		fileList *next_node = dups_list_h->next->next->next, *cur_node =dups_list_h->next->next; // cur_node를 끼워넣기 
		start->next->next = NULL;
		
		while(size--){
			fileList *cur = start->next, *prev = start; // start 순차적으로 돌면서 맞는 위치 넣기 
			cur_node->next = NULL;
			if(order==1){ // 오름차순 
			
				while(cur != NULL && cur->filesize <= cur_node->filesize){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
			}else{ // 내림차순 
				while(cur != NULL && cur->filesize >= cur_node->filesize){
					prev = cur;
            				cur = cur->next;
				}
				cur_node->next = cur;
        			prev->next = cur_node;
			}
			
			cur_node = next_node;
			if(next_node == NULL)
				break;
			next_node = next_node->next;
		}
		dups_list_h = start;

	}
}

void sortList(int cate, int type, int order){
	if(type==1)
		sort_list(cate, order);
	else
		sort_set(cate, order);
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
int filelist_size(fileList *head)
{
	fileList *cur = head->next;
	int size = 0;
	
	while (cur != NULL){
		size++;
		cur = cur->next;
	}
	
	return size;
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
void filesize_with_comma(long long filesize, char *filesize_w_comma)
{
	char filesize_wo_comma[STRMAX] = {0, };
	int comma;
	int idx = 0;

	sprintf(filesize_wo_comma, "%lld", filesize);
	comma = strlen(filesize_wo_comma)%3;

	for (int i = 0 ; i < strlen(filesize_wo_comma); i++){
		if (i > 0 && (i%3) == comma)
			filesize_w_comma[idx++] = ',';

		filesize_w_comma[idx++] = filesize_wo_comma[i];
	}

	filesize_w_comma[idx] = '\0';
}

void sec_to_ymdt(struct tm *time, char *ymdt)
{
	sprintf(ymdt, "%04d-%02d-%02d %02d:%02d:%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}
void filelist_print_format(fileList *head) // 파일리스트 헤드를 인자로 받음 
{
	fileList *filelist_cur = head->next; // 한칸 이동 
	int set_idx = 1;	

	while (filelist_cur != NULL){ // 리스트 끝까지 이동 
		
		fileInfo *fileinfolist_cur = filelist_cur->fileInfoList->next;
		
		char atime[STRMAX];
		char mtime[STRMAX];
		char filesize_w_comma[STRMAX] = {0, };
		int i = 1;

		filesize_with_comma(filelist_cur->filesize, filesize_w_comma);

		printf("---- Identical files #%d (%s bytes - %s) ----\n", set_idx++, filesize_w_comma, filelist_cur->hash);

		while (fileinfolist_cur != NULL){
			sec_to_ymdt(localtime(&fileinfolist_cur->statbuf.st_mtime), mtime);
			sec_to_ymdt(localtime(&fileinfolist_cur->statbuf.st_atime), atime);
			printf("[%d] %s (mtime : %s) (atime : %s) (uid : %d) (gid : %d) (mode : %o)\n", i++, fileinfolist_cur->path, mtime, atime,fileinfolist_cur->statbuf.st_uid,fileinfolist_cur->statbuf.st_gid,fileinfolist_cur->statbuf.st_mode);

			fileinfolist_cur = fileinfolist_cur->next;
		}

		printf("\n");

		filelist_cur = filelist_cur->next;
	}	
}
int make_log(char* command, char* path, char* time,char* usrname){
	FILE* fp;
	if(getuid()==0){ // 루트 권한 
	
	if ((fp = fopen("/.duplicate_20202912.log", "a")) == NULL){
		printf("ERROR: fopen error for %s\n", "/.duplicate_20202912.log");
		return 1;
	}
	}else{ // 일반 사용자 권한 
	char log_path[PATHMAX];
	get_path_from_home("~/.duplicate_20202912.log", log_path);
	
	if ((fp = fopen(log_path, "a")) == NULL){
		printf("ERROR: fopen error for %s\n", "~/.duplicate_20202912.log");
		return 1;
	}
	}
	
	fprintf(fp, "[%s] %s %s %s\n",command,path, time, usrname);
	fclose(fp);
	return 0;
}
int filelist_search(fileList *head, char *hash) // 같은 해시값 찾는 함수 
{
	fileList *cur = head;
	int idx = 0;

	while (cur != NULL){
		if (!strcmp(cur->hash, hash))
			return idx;
		cur = cur->next;
		idx++;
	}

	return 0;
}
void filelist_delete_node(fileList *head, char *hash) // 파일리스트 삭제 
{
	fileList *deleted;

	if (!strcmp(head->next->hash, hash)){ // 같은 해시의 파일리스트 찾기 
		deleted = head->next;
		head->next = head->next->next;
	}
	else {
		fileList *filelist_cur = head->next;

		while (filelist_cur->next != NULL){ // 순회 
			if (!strcmp(filelist_cur->next->hash, hash)){ // 해시값 같다면 
				deleted = filelist_cur->next;
				
				filelist_cur->next = filelist_cur->next->next;

				break;
			}

			filelist_cur = filelist_cur->next;
		}
	}

	free(deleted);
}
int get_dirlist(char *target_dir, struct dirent ***namelist) // 디렉토리 경르  
{
	int cnt = 0;

	if ((cnt = scandir(target_dir, namelist, NULL, alphasort)) == -1){ // 디렉토리 이름순 소팅  
		printf("ERROR: scandir error for %s\n", target_dir);
		return -1;
	}

	return cnt; //dir 개수 리턴  
}
int fileinfolist_size(fileInfo *head)
{
	fileInfo *cur = head->next;
	int size = 0;
	
	while (cur != NULL){
		size++;
		cur = cur->next;
	}
	
	return size;
}

void get_same_size_files_dir(void)
{	
	if(getuid()==0){ // 루트 사용자라면 
		strcpy(same_size_files_dir, "/.Trash/info/"); //절대경로 받아내기 (중복 파일 적어놓기)
		
	}else{ // 일반 사용자라면 
		get_path_from_home("~/.Trash/info", same_size_files_dir); //절대경로 받아내기 (중복 파일 적어놓기)
	}
	

	if (access(same_size_files_dir, F_OK) == 0){ // 존재한다면 비우기 
		remove_files(same_size_files_dir);
	
	}else{
		if(mkdir(same_size_files_dir, 0755)<0)
		printf("file make errorn");
		/*if (access(same_size_files_dir, F_OK) == 0) // 존재한다면 삭제후 만들기 
			printf("make dir %s ok\n", same_size_files_dir);
		else
			printf("make fail");*/
			}
			
	
}

void find_duplicates(void) // 링크드리스트 제작 
{
	struct dirent **namelist;
	int listcnt;
	char hash[HASHMAX];
	FILE *fp;

	listcnt = get_dirlist(same_size_files_dir, &namelist); // 이름 받아오기 

	for (int i = 0; i < listcnt; i++){ // 안의 파일 읽기 
		char filename[PATHMAX*2];
		long long filesize;
		char filepath[PATHMAX];
		char hash[HASHMAX];
		char line[STRMAX];

		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

		filesize = atoll(namelist[i]->d_name); // 사이즈 받기 
		sprintf(filename, "%s/%s", same_size_files_dir, namelist[i]->d_name); // filename에 이름과 사이즈 저장 

		if ((fp = fopen(filename, "r")) == NULL){ //오픈 
			printf("ERROR: fopen error for %s\n", filename);
			continue;
		}

		while (fgets(line, sizeof(line), fp) != NULL){
			int idx;

			strncpy(hash, line, HASHMAX); // 각각 불러오기 
			hash[HASHMAX-1] = '\0';

			strcpy(filepath, line+HASHMAX);
			filepath[strlen(filepath)-1] = '\0';

			if ((idx = filelist_search(dups_list_h, hash)) == 0) // 해시 값 같으면 
				filelist_append(filesize, filepath, hash); // 그 곳에 붙어넣기 
			else{ // 같은 해시값 없음 
				fileList *filelist_cur = dups_list_h;
				while (idx--){
					filelist_cur = filelist_cur->next;
				}
				fileinfo_append(filelist_cur->fileInfoList, filepath); // 맨 끝에 넣어줌 
			}
		}

		fclose(fp);
	}
}
void remove_no_duplicates(void)
{
	fileList *filelist_cur = dups_list_h->next; //순환할 리스트 포인터 

	while (filelist_cur != NULL){
		fileInfo *fileinfo_cur = filelist_cur->fileInfoList;

		if (fileinfolist_size(fileinfo_cur) < 2) // 리스트 안 자료가 2개 미만이면 삭제 
			filelist_delete_node(dups_list_h, filelist_cur->hash);
		
		filelist_cur = filelist_cur->next;
	}
}
void get_filename(char *path, char *filename)
{
	char tmp_name[NAMEMAX];
	char *pt = NULL;

	memset(tmp_name, 0, sizeof(tmp_name));
	
	if (strrchr(path, '/') != NULL)
		strcpy(tmp_name, strrchr(path, '/') + 1);
	else
		strcpy(tmp_name, path);
	
	if ((pt = get_extension(tmp_name)) != NULL)
		pt[-1] = '\0';

	if (strchr(path, '/') == NULL && (pt = strrchr(tmp_name, '.')) != NULL)
		pt[0] = '\0';
	
	strcpy(filename, tmp_name);
}



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int front=-1;
int back=-1;
char queue[QUEUE_SIZE][PATH_MAX] = {0,};
void push(char* dir){ // 큐에 push
	
	back = (back+1)%QUEUE_SIZE;
	
	if(back==front){
		fprintf(stderr, "queue is full");
		exit(1);
	}
	
	strcpy(queue[back], dir);
	
}
void *sthread(void *arg){
	DIR *dirp; // 디렉토리 포인터
	
	while(front!=back){ // 큐 빌때까지 순환 
		
		if((dirp = opendir(queue[(front+1)%QUEUE_SIZE]))==NULL){ // 디렉토리 오픈 
			fprintf(stderr, "dir open error!");
			exit(1);
		}
		//printf("%d %d %d\n",front, back,(front+1)%QUEUE_SIZE);
		struct dirent *dentry;
		while((dentry = readdir(dirp))!=NULL){ // 디렉토리 안 파일 순회 
			char fullpath[PATHMAX] = {0, }; // 안의 파일 절대경로 저장 
			struct stat statbuf;
			if (!strcmp(dentry->d_name, ".") || !strcmp(dentry->d_name, "..")) // 현위치 디렉토리, 전 디렉토리 건너뛰기 
				continue;
			get_fullpath(queue[(front+1)%QUEUE_SIZE], dentry->d_name, fullpath); // 절대경로 저장 
			
			if (!strcmp(fullpath,"/proc") || !strcmp(fullpath, "/run") || !strcmp(fullpath, "/sys") || !strcmp(fullpath, "/sys")) // 건너뛸 파일들 스킵 
				continue;
			if (lstat(fullpath, &statbuf) < 0){ // stat 가져오기 
				printf("ERROR: lstat error for %s\n", fullpath);
				pthread_mutex_lock(&mutex);
				front = (front+1)%QUEUE_SIZE;
				pthread_mutex_unlock(&mutex);
				return;
			}
			

			if(S_ISDIR(statbuf.st_mode)){ // 디렉토리라면 
				pthread_mutex_lock(&mutex);
				push(fullpath);
				pthread_mutex_unlock(&mutex);	
			}else if(S_ISREG(statbuf.st_mode)){ // 일반 파일이라면 
				//printf("%s\n", fullpath);
				FILE *fp;
				char filename[PATHMAX*2];
				char *path_extension; // 확장자 저장 
				char hash[HASHMAX]; // 해시값 저장 
				int filesize = (long long)statbuf.st_size;
			if((long long)statbuf.st_size<minbsize) // 사이즈 검사 
				continue;
			if(maxbsize!=-1 && (long long)statbuf.st_size>maxbsize)
				continue;
				sprintf(filename, "%s/%lld", same_size_files_dir, filesize);
				memset(hash, 0, HASHMAX); // 헤시 초기화
				md5(fullpath, hash); // 헤시 할당 
				path_extension = get_extension(fullpath); // 확장자 가져오기 
				if (strlen(extension) != 0){ // 확장자 처리 
					if (path_extension == NULL || strcmp(extension, path_extension))
						continue;
				}
				if ((fp = fopen(filename, "a")) == NULL){
					printf("ERROR: fopen error for %s\n", filename);
					pthread_mutex_lock(&mutex);
			front = (front+1)%QUEUE_SIZE;
		pthread_mutex_unlock(&mutex);
					return;
				}

				fprintf(fp, "%s %s\n", hash, fullpath);

				fclose(fp);
				//printf("%s %s %d\n", hash, fullpath, (long long) statbuf.st_size);
				//add_list(hash, fullpath,(long long)statbuf.st_size);

			}

		}
		closedir(dirp);
		pthread_mutex_lock(&mutex);
		front = (front+1)%QUEUE_SIZE;
		pthread_mutex_unlock(&mutex);
		
		
	}
	
	pthread_exit(NULL);
	return NULL;
}
int make_list(){
pthread_mutex_init(&mutex,NULL);
	front = -1; // 큐 위치 
	back = -1;
	push(target);
	int status;
	
	dups_list_h = (fileList *)malloc(sizeof(fileList));
	dups_list_h->next = NULL;
	pthread_t tid[5];
	
	for(int i=0;i<thread_num;i++){
	if(pthread_create(&tid[i],NULL,sthread,NULL)!=0){
		fprintf(stderr, "thread creat error");
		exit(1);
	
	}
	sleep(0.1);
	
	} 
	
	for(int i=0;i<thread_num;i++){
		pthread_join(tid[i],NULL);
	
	} 
	status = pthread_mutex_destroy(&mutex);
}
