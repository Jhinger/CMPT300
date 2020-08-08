#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

#define MAX_FILES 1024
#define MAX_NAME_LEN 1024

//Struct with array storing subdirectories and size of the array.
typedef struct dirInfo {
    struct dirent subdirectories[MAX_FILES];
    int size;
} directoryInformation;

//Returns an array of directories in currentDirectory.
directoryInformation findDirectories(char * currentDirectory);

//Have an exit clause that exits the function once no directories are found?
void depthSearch(char * path);

void testPrintDeleteLater();

int checkHiddenFile(char * name);

#endif