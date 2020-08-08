#ifndef _INFORMATION_H_
#define _INFORMATION_H_

#define MAX_NAME_LEN 1024

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

int getFileInfo(char * pathName, struct stat * fileInfo);

void printFileInfo(struct stat fileInfo, char * fileName);

void printFilesInDirectory(char* directory);

void sortDirStruct(struct dirent array[],int length);

void longListingCurr(char* directory);

void getAndPrintGroup(gid_t grpNum);

void getAndPrintUserName(uid_t uid);

void printDate(time_t date);

void printMode(mode_t mode);

void printInode(char* directory);

#endif