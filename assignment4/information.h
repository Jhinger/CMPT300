#ifndef _INFORMATION_H_
#define _INFORMATION_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int getFileInfo(char * pathName, struct stat * fileInfo);

void printFileInfo(struct stat fileInfo, char * fileName);

#endif