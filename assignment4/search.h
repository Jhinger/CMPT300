#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include<stdbool.h>

#define MAX_FILES 4096
#define MAX_NAME_LEN 4096

void depthSearch(char * path, bool ilR_Array[]);

int checkHiddenFile(char * name);

void freeArrays(char ** folderArray, struct dirent ** directoryArray, int folderCount);

void printFormat(char * nextPath, int length, char * currentPath, char * filename);

#endif