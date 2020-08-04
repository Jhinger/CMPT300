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
    printf("%-10ld ",(long) fileInfo.st_ino);
    printf("%-10lu ", (unsigned long) fileInfo.st_mode);
    printf("%-5ld ", (long) fileInfo.st_nlink);
    printf("%-10lu ", (unsigned long) fileInfo.st_uid);
    printf("%-10lu ", (unsigned long) fileInfo.st_gid);
    printf("%-10li ", fileInfo.st_size);
    printf("%-5li ", fileInfo.st_blocks);
    printf("%-10s\n", fileName);
}

int getLastOptionIndex(int numArguments, char * arguments[]) {
    int lastOption = -1;

    //If there are no options given.
    if (numArguments == 1) {
        return lastOption;
    }

    //Otherwise loop through until encounter first non-option.
    for (int i = 1; i < numArguments; i++) {
        if (strcmp(arguments[i][0], '-') != 0) {
            break;
        }
        lastOption = i;
    }
    return lastOption;
}