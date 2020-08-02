#include <stdio.h>
#include <time.h>
#include "information.h"

int getFileInfo(char * pathName, struct stat * fileInfo) {
    if (lstat(pathName, fileInfo) != 0) {
        perror("Error accessing lstat in file information.c\n");
        return -1;
    }
    return 0;
}

void printFileInfo(struct stat fileInfo, char * fileName) {
    printf("File: %s\n", fileName);
    printf("%-6ld :Inode\n",(long) fileInfo.st_ino);
    printf("%-6lu :Mode\n", (unsigned long) fileInfo.st_mode);
    printf("%-6ld :Hard Links\n", (long) fileInfo.st_nlink);
    printf("%-6lu :Owner ID\n", (unsigned long) fileInfo.st_uid);
    printf("%-6lu :Group ID\n", (unsigned long) fileInfo.st_gid);
    printf("%-6li :Size(Bytes)\n", fileInfo.st_size);
    printf("%-6li :Blocks allocated\n", fileInfo.st_blocks);
}