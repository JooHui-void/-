#include <stdio.h>
#include "ssu_sfinder.h"
void help(){ 
    printf("Usage:\n");
    printf("  > fmd5/fsha1 [FILE_EXTENSION] -I [MINSIZE] -h [MAXSIZE] -d [TARGET_DIRECTORY] -t [THREAD_NUM]\n");
    printf("     >> delete -I [SET_INDEX] -d [OPTION] -i -f -t\n");
    printf("  > trash -c [CATEGORY] -o [ORDER]\n");
    printf("  > restore [RESTORE_INDEX]\n");
    printf("  > help\n");
    printf("  > exit\n\n");
}
