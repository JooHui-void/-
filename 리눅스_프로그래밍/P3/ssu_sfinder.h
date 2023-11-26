#ifndef HEAD
#define HEAD


#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <openssl/md5.h>
#include <time.h>
#include <pthread.h>

#define STRMAX 10000 
#define ARGMAX 15
#define NAMEMAX 255
#define PATHMAX 4096
#define KB 1000
#define MB 1000000
#define GB 1000000000
#define SIZE_ERROR -2
#define HASHMAX 33


typedef struct fileInfo {
	char path[PATHMAX];
	struct stat statbuf;
	time_t trash_time;
	struct fileInfo *next;
} fileInfo;

typedef struct fileList {
	long long filesize;
	char hash[HASHMAX];
	int filenum;
	fileInfo *fileInfoList;
	struct fileList *next;
} fileList;

typedef struct dirList {
	char dirpath[PATHMAX];
	struct dirList *next;
} dirList;
void sortList();
void filelist_print_format(fileList *head);
void find_duplicates();
void remove_no_duplicates(void);
void sdelete(int argc, char *argv[]);
void get_same_size_files_dir(void);
void get_trash_path();
void slist(int argc, char *argv[]);
char *get_extension(char *filename);
int make_list();
void printList();
int fileinfolist_size(fileInfo *head);
void sec_to_ymdt(struct tm *time, char *ymdt);
void get_filename(char *path, char *filename);
int make_log(char* command, char* path, char* time,char* usrname);
int filelist_size(fileList* );
void filelist_delete_node(fileList *head, char *hash);
fileInfo *fileinfo_delete_node(fileInfo *head, char *path);
void get_path_from_home(char *path, char *path_from_home);
void remove_files(char *dir);
#endif
