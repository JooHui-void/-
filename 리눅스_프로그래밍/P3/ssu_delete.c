#include "ssu_sfinder.h"

int param_opt;
int removePos; // 리스트 세트 번호 
int list_idx; // 리스트 인덱스 번호 
int removeoption; // 지우는 옵션 
extern fileList* dups_list_h;

char trash_path[PATHMAX];
char log_path[PATHMAX];
extern fileInfo *trash_list;
void remove_files(char *dir)
{
	struct dirent **namelist;
	int listcnt = get_dirlist(dir, &namelist);

	for (int i = 0; i < listcnt; i++){
		char fullpath[PATHMAX] = {0, };

		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

		get_fullpath(dir, namelist[i]->d_name, fullpath);

		remove(fullpath);
	}
}
void get_new_file_name(char *org_filename, char *new_filename)
{
	char new_trash_path[PATHMAX];
	char tmp[NAMEMAX];
	struct dirent **namelist;
	int trashlist_cnt;
	int same_name_cnt = 1;

	get_filename(org_filename, new_filename);
	trashlist_cnt = get_dirlist(trash_path, &namelist);

	for (int i = 0; i < trashlist_cnt; i++){
		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;

		memset(tmp, 0, sizeof(tmp));
		get_filename(namelist[i]->d_name, tmp);

		if (!strcmp(new_filename, tmp))
			same_name_cnt++;
	}
	
	sprintf(new_filename + strlen(new_filename), ".%d", same_name_cnt);

	if (get_extension(org_filename) != NULL)
		sprintf(new_filename + strlen(new_filename), ".%s", get_extension(org_filename));
}

time_t get_recent_mtime(fileInfo *head, char *last_filepath) // 최근 수정파일 구하기 
{
	fileInfo *fileinfo_cur = head->next;
	time_t mtime = 0;

	while (fileinfo_cur != NULL){
		if (fileinfo_cur->statbuf.st_mtime > mtime){
			mtime = fileinfo_cur->statbuf.st_mtime;
			strcpy(last_filepath, fileinfo_cur->path);
		}
		fileinfo_cur = fileinfo_cur->next;
	}
	return mtime;
}


void get_trash_path(void) //trash_path 에다 쓰레기통 위치 저장 
{	
	if (getuid() == 0){ //루트 유저라면 
		strcpy(trash_path,"/.Trash/files/"); // 휴지통 위치 

	} 
	else  // 일반 사용자라면 
		get_path_from_home("~/.Trash/files/", trash_path);
	if (access(trash_path, F_OK) == 0) // 존재한다면 삭제 
		remove_files(trash_path);
	else
		mkdir(trash_path, 0755);
}



void sdelete(int argc, char *argv[]){
	param_opt = 0;
	removePos =0;
	list_idx = 0;
	removeoption = 0;
	optind = 0;
	opterr = 0;
	time_t curTime = time(NULL);
	struct passwd *pw;
	pw = getpwent();
	char timenow[STRMAX];
	char last_filepath[PATHMAX]; //최근수정파일 이름 저장 
	char modifiedtime[STRMAX]; // 시간 문자열 
	get_trash_path();
	while((param_opt = getopt(argc, argv, "l:d:ift"))!=-1){ // 옵션 처리 
		switch(param_opt){
			case 'l' :  // 삭제하고자 하는 리스트 세트 번호 
				removePos = atoi(optarg);
				break;
			case 'd' : // 해당 넘버 삭제 
				if(argc!=5){
					printf("usage error");
					return;
				}
				list_idx = atoi(optarg);
				removeoption = 'd';
				break;
			case 'i' : // 파일 하나씩 삭제여부 판단 
				if(argc!=4){
					printf("usage error");
					return;
				}
				removeoption = 'i';

				break;
			case 'f' : // 최근 수정파일 남겨두고 다 삭제 
				if(argc!=4){
					printf("usage error");
					return;
				}
				removeoption = 'f';

				break;
			case 't' : // 가장 최근 수정파일 남기고 나머지 휴지통 이동  
				if(argc!=4){
					printf("usage error");
					return;
				}


				removeoption = 't';
				break;
			case '?' : // 모르는 문자열 
				printf("unknowned option\n");
				return;
		}
	}
	if(removePos==0||removeoption ==0){ // 입력 에러 처리 
		printf("usage error, -l option missing");
		return;
	}

	if(removePos<1 ||filelist_size(dups_list_h)<removePos){ // 파일 세트 사이즈 에러 
		printf("file size error");
		return;
	}
	int set_idx = removePos; // 임시 이동 변수 
	fileList *target_filelist_p = dups_list_h->next; // 지울 세트로 갈 포인터 
	while (--set_idx) // -l 옵션에 따른 목적지까지 이동 
		target_filelist_p = target_filelist_p->next;
	
	fileInfo* target_infolist_p = target_filelist_p->fileInfoList; // fileinfo 포인터 할당 
	set_idx = removePos;
	
	time_t mtime = get_recent_mtime(target_infolist_p, last_filepath); // 리스트에서 최근 수정 파일 이름 저장 
	sec_to_ymdt(localtime(&mtime), modifiedtime);
	
	if(removeoption == 'd'){ // 해당 넘버 하나만 삭제
		fileInfo *deleted; // 

		if (list_idx < 1 || list_idx > fileinfolist_size(target_infolist_p)){  // 인덱스 에러 
			printf("ERROR: [LIST_IDX] out of range\n");
			return;
		}

		deleted = target_infolist_p; // 세트 맨 앞 info 가리킴 

		while (list_idx--)
			deleted = deleted->next; // 해당하는 인덱스로 이동 

		printf("\"%s\" has been deleted in #%d\n\n", deleted->path, removePos);
		curTime = time(NULL);
		sec_to_ymdt(localtime(&curTime),timenow);
		
		make_log("DELETE", deleted->path,timenow,pw->pw_name);
		
		remove(deleted->path); // 진짜 지우기 
		fileinfo_delete_node(target_infolist_p, deleted->path); // 링크드리스트 지우기 

		if(fileinfolist_size(target_infolist_p) < 2) // 1개면 싹 다 지우기 
			filelist_delete_node(dups_list_h, target_filelist_p->hash);
	}else if(removeoption == 'i'){ // 파일 하나씩 삭제 판단 
		char ans[STRMAX]; 
			fileInfo *fileinfo_cur = target_infolist_p->next; // 리스트 이동 포인터 
			fileInfo *deleted_list = (fileInfo *)malloc(sizeof(fileInfo)); //삭제 리스트 
			fileInfo *tmp;
			int listcnt = fileinfolist_size(target_infolist_p); // 리스트 사이즈 


			while (fileinfo_cur != NULL && listcnt--){  // 리스트 순회 
				printf("Delete \"%s\"? [y/n] ", fileinfo_cur->path);
				memset(ans, 0, sizeof(ans));
				fgets(ans, sizeof(ans), stdin);

				if (!strcmp(ans, "y\n") || !strcmp(ans, "Y\n")){ // 삭제 
				curTime = time(NULL);
					sec_to_ymdt(localtime(&curTime),timenow);
					make_log("DELETE", fileinfo_cur->path,timenow,pw->pw_name);
					remove(fileinfo_cur->path);
					fileinfo_cur = fileinfo_delete_node(target_infolist_p, fileinfo_cur->path);				
				} 
				else if (!strcmp(ans, "n\n") || !strcmp(ans, "N\n")) // 삭제 안함 
					fileinfo_cur = fileinfo_cur->next;										
				else {
					printf("ERROR: Answer should be 'y/Y' or 'n/N'\n");
					break;
				}
			}
			printf("\n");

			if (fileinfolist_size(target_infolist_p) < 2)
				filelist_delete_node(dups_list_h, target_filelist_p->hash);
	}else if(removeoption == 'f'){ // 최근 수정파일 남겨두고 다 삭제 
fileInfo *tmp;				
			fileInfo *deleted = target_infolist_p->next; //순회 포인터 

			while (deleted != NULL){
				tmp = deleted->next;
				
				if (!strcmp(deleted->path, last_filepath)){ // 최근 파일 건너뛰기 
					deleted = tmp;
					continue;
				}
				curTime = time(NULL); 
				sec_to_ymdt(localtime(&curTime),timenow); // 시간 받아오기 
				make_log("DELETE", deleted->path,timenow,pw->pw_name);
				remove(deleted->path); // 삭제 
				free(deleted); // 링크드리스트 삭제 
				deleted = tmp;
			}

			filelist_delete_node(dups_list_h, target_filelist_p->hash); // 링크드리스트 세트 삭제 
			printf("Left file in #%d : %s (%s)\n\n",removePos, last_filepath, modifiedtime);
	}else if(removeoption == 't'){ // 최근 수정파일 남기고 휴지통 이동 
		fileInfo *tmp;
			fileInfo *deleted = target_infolist_p->next; // 이동 포인터 
			char move_to_trash[PATHMAX];
			char filename[PATHMAX];

			while (deleted != NULL){ // 끝까지 이동 
				tmp = deleted->next;
				
				if (!strcmp(deleted->path, last_filepath)){ // 최근 파일과 이름이 같으면 넘어감 
					deleted = tmp;
					continue;
				}

				memset(move_to_trash, 0, sizeof(move_to_trash)); // 휴지통 위치 리셋 
				memset(filename, 0, sizeof(filename));
				
				sprintf(move_to_trash, "%s%s", trash_path, strrchr(deleted->path, '/') + 1); // 휴지통 위치 세팅 

				if (access(move_to_trash, F_OK) == 0){  // 접근 가능 
					get_new_file_name(deleted->path, filename);

					strncpy(strrchr(move_to_trash, '/') + 1, filename, strlen(filename)); // / 포함 앞까지 복사 
				}
				else
					strcpy(filename, strrchr(deleted->path, '/') + 1); 
				
				
				curTime = time(NULL);
				sec_to_ymdt(localtime(&curTime),timenow);
				make_log("REMOVE", deleted->path,timenow,pw->pw_name);
				fileinfo_append(trash_list,deleted->path);
				if (rename(deleted->path, move_to_trash) == -1){ // 이름 휴지통으로 교체 
					printf("ERROR: Fail to move duplicates to Trash\n");
					continue;
				}
				
				free(deleted);
				deleted = tmp;
			}

			filelist_delete_node(dups_list_h, target_filelist_p->hash); // 노드 삭제 
			printf("All files in #%d have moved to Trash except \"%s\" (%s)\n\n", removePos, last_filepath, modifiedtime);
	}else {
		printf("error");
		return;
	}	

}
