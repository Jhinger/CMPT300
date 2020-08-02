#include <stdio.h>
#include <string.h>
#include "information.h"

int main(int numArgs, char * args[]) {
    char * fileName = args[1];
    struct stat fileInfo;
    int check = 0;

    if ((check = getFileInfo(fileName, &fileInfo)) != 0) {
        perror("Error getting file information.\n");
    }

    printFileInfo(fileInfo, fileName);
}