#include <stdio.h>
#include "ssu_sindex.h"


char command[100];
void main(){
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);

	while(1){
		printf("20202912> ");
		gets(command);
		if(strlen(command)==0){
			continue;
		}
		char *ptr = strtok(command," ");
		if(strcmp(ptr,"find")==0){
			char *ptr2 = strtok(NULL," ");
			char *ptr3 = strtok(NULL," ");
			find(ptr2,ptr3);
			getchar();
			continue;
		}else if(strcmp(ptr,"exit")==0){
			printf("Prompt End\n");
			printf("Runtime : ");
			gettimeofday(&end,NULL);
			long gap_sec = end.tv_sec-start.tv_sec;
			long gap_usec = end.tv_usec-start.tv_usec;
			if(gap_usec<0){
				gap_sec -=1;
				gap_usec +=1000000;
			}
			printf("%ld:%ld(sec:usec)\n",gap_sec,gap_usec);
			return 0;
		}else{
			help();
		}
	}
}
