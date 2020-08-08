#include <stdio.h>
#include <time.h>
#include "information.h"
#include <dirent.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>

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

void printFilesInDirectory(char* directory){ // this is if user does ls command only
    DIR *dir;
    struct dirent *dirStruct;
    dir = opendir(directory);//we can pass in different directories here!*********************************

    int length = 0;
    struct dirent dirStructArray[50];
    while((dirStruct = readdir(dir)) != NULL){
        dirStructArray[length] = *dirStruct;
        length++;
    }
    sortDirStruct(dirStructArray,length);

    int i = 0;
    while(i < length){
        printf("%s\n", dirStructArray[i].d_name);
        i++;
    }
}

void sortDirStruct(struct dirent array[],int length){
    for(int i = 0; i < length-1; i++){
        int index = i;
        for(int j = i+1; j <length; j++){
            char* minName = array[index].d_name;
            char* currentName = array[j].d_name;
            if(strcmp(currentName,minName)<= 0){
                index = j;
            }
        }
        struct dirent temp = array[i];
        array[i] = array[index];
        array[index] = temp;
    }
}

void longListingCurr(char* directory){
    DIR *dir;
    struct dirent *dirStruct;
    dir = opendir(directory);//we can pass in different directories here!*********************************

    
    int length = 0;
    struct dirent dirStructArray[70];
    while((dirStruct = readdir(dir)) != NULL){
        dirStructArray[length] = *dirStruct;
        length++;
    }
    sortDirStruct(dirStructArray,length);

    int i = 0;
    struct stat fileInfo;
    while(i < length){
        char temp[100];
        strcpy(temp,directory);
        strcat(temp,"/");
        strcat(temp,dirStructArray[i].d_name);

        
         getFileInfo(temp,&fileInfo);
         printMode(fileInfo.st_mode);
         printf("%ld ", (long) fileInfo.st_nlink);
         getAndPrintUserName(fileInfo.st_uid);
         getAndPrintGroup(fileInfo.st_gid);
         printf("%5li ", fileInfo.st_size); // we need to fix this padding
         printDate(fileInfo.st_mtime);
         printf("%s ", dirStructArray[i].d_name);
         printf("\n");
        i++;
    }
}

void getAndPrintGroup(gid_t grpNum)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
        //printf("The group ID %u -> %s\n", grpNum, grp->gr_name);
        printf("%s ",grp->gr_name);
    } else {
        printf("No group name for %u found\n", grpNum);
    }
}

void getAndPrintUserName(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        //printf("The user ID %u -> %s", uid, pw->pw_name);
        printf("%s ",pw->pw_name);
    } else {
        perror("Hmm not found???");
        printf("No name found for %u\n", uid);
    }
}

void printDate(time_t date){
char dateArr[19];
strftime(dateArr, 19, "%b %e %Y %H:%M", localtime(&date));
printf("%s ",dateArr);

}

void printMode(mode_t mode){
    //help recieved for printing mode from Might have to change do not know if allowed to copy this
    //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
    printf( (S_ISDIR(mode)) ? "d" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

void printInode(char* directory){
    DIR *dir;
    struct dirent *dirStruct;
    dir = opendir(directory);//we can pass in different directories here!*********************************

    int length = 0;
    struct dirent dirStructArray[70];
    while((dirStruct = readdir(dir)) != NULL){
        dirStructArray[length] = *dirStruct;
        length++;
    }
    sortDirStruct(dirStructArray,length);

    int i = 0;
    struct stat fileInfo;
    while(i < length){
        char temp[100];
        strcpy(temp,directory);
        strcat(temp,"/");
        strcat(temp,dirStructArray[i].d_name);
         getFileInfo(temp,&fileInfo);
         
         printf("%ld ",(long) fileInfo.st_ino);
         printf("File: %s\n", dirStructArray[i].d_name);
         i++;
         
    }
}