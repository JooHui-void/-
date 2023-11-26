#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <sys/time.h>
#include <time.h>
#include <openssl/sha.h>
#define NAMESIZE 255
#define QUEUE_MAX 1000

typedef struct setnode{
	char *name;
	struct setnode* ptr;
}setnode;



/*
void fmd5();
void fsha1();
void help();
void PrintTime(struct stat *);
int Full();
void push(char*);
void MakeBound(int, char*);
setnode* MakeSetnode(char*);
void FileDelete(set*, setnode *, setnode* );
void PrintAll();
void RemoveList();
void file_init();

*/


/*
char queue[QUEUE_MAX][4096];
int file_max;
int file_min;
int runcnt;
struct tm *time_info;
set* set_head;
set* set_tail;
int front;
int rear;
unsigned char data[4096];
char *file_extension[10];
char buf[4096];
setnode *sn;
setnode *snn;
*/
/*
extern char queue[QUEUE_MAX][4096];
extern int file_max = -1;
extern int file_min = -1;
extern int runcnt;
extern struct tm *time_info;
extern set* set_head = NULL;
extern set* set_tail = NULL;
extern int front = -1;
extern int rear = -1;
extern unsigned char data[4096];
extern char *file_extension[10];
extern char buf[4096];
extern setnode *sn;
extern setnode *snn;
*/
