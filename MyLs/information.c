#include <stdio.h>
#include <time.h>
#include "information.h"
#include <dirent.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include<stdbool.h>
#include "search.h"

int getFileInfo(char * pathName, struct stat * fileInfo) {
    if (lstat(pathName, fileInfo) != 0) {
        return -1;
    }
    return 0;
}

bool hasInvalidChar(char * name) {
    if (strstr(name, " ") != NULL ||
        strstr(name, "!") != NULL ||
        strstr(name, "$") != NULL ||
        strstr(name, "^") != NULL ||
        strstr(name, "&") != NULL ||
        strstr(name, "(") != NULL ||
        strstr(name, ")") != NULL
    )   return true;
    return false;
}

void universalPrint(char* path, char* name, bool ilR_Array[],int padding, int largestFileDigit){
    char *temp = (char *)malloc(1024 * sizeof(char));
    char *buf = (char *)malloc(1024 * sizeof(char));
    strcpy(temp,path);
    strcat(temp,"/");
    strcat(temp,name);

    struct stat fileInfo;
    getFileInfo(temp,&fileInfo);

    if (ilR_Array[0]) {
        printf("%*.llu ",padding,(unsigned long long) fileInfo.st_ino);
    }
    if (ilR_Array[1]) {
        printMode(fileInfo.st_mode);
        printf("%llu ", (unsigned long long) fileInfo.st_nlink);
        getAndPrintUserName(fileInfo.st_uid);
        getAndPrintGroup(fileInfo.st_gid);
        
        printf("%*li ", largestFileDigit, fileInfo.st_size); 
        printDate(fileInfo.st_mtime);
    }

    int bytesRead = readlink(temp, buf, 1024);
    //If it's not a symbolic link.
    if (bytesRead == -1) {
        if (hasInvalidChar(name)) {
            printf("'%s' ", name);
        } else {
            printf("%s ", name);
        }
    } else {
        //Otherwise if it is a symbolic link.
        buf[bytesRead] = '\0';
        if (hasInvalidChar(name) && hasInvalidChar(buf)) {
            printf("'%s' -> '%s'", name, buf);
        } else if (hasInvalidChar(name) && !hasInvalidChar(buf)) {
            printf("'%s' -> %s", name, buf);
        } else if (!hasInvalidChar(name) && hasInvalidChar(buf)) {
            printf("%s -> '%s'", name, buf);
        } else if (!hasInvalidChar(name) && !hasInvalidChar(buf)) {
            printf("%s -> %s", name, buf);
        }
    }  
    printf("\n");
    free(temp);
    free(buf);
}

void printFilesInDirectory(char* directory){ // this is if user does ls command only
    DIR *dir;
    struct dirent *dirStruct;
    dir = opendir(directory);

    int length = 0;
    struct dirent dirStructArray[50];
    while ((dirStruct = readdir(dir)) != NULL){
        dirStructArray[length] = *dirStruct;
        length++;
    }
    sortDirStruct(dirStructArray,length);

    int i = 0;
    while(i < length){
        printf("%s\n", dirStructArray[i].d_name);
        i++;
    }
    closedir(dir);
}

void sortDirStruct(struct dirent array[],int length){
    for (int i = 0; i < length-1; i++){
        int index = i;
        for (int j = i+1; j <length; j++){
            char* minName = array[index].d_name;
            char* currentName = array[j].d_name;
            if (strcmp(currentName,minName)<= 0){
                index = j;
            }
        }
        struct dirent temp = array[i];
        array[i] = array[index];
        array[index] = temp;
    }
}

void getDirectoryFiles(char* directory,bool ilR_Array[]){
    DIR *dir;
    struct dirent *dirStruct;

    if ((dir = opendir (directory)) == NULL) {
        perror("Cannot open ");
        exit(1);
    }

    int length = 0;
    struct dirent dirStructArray[70];
    while ((dirStruct = readdir(dir)) != NULL){
        dirStructArray[length] = *dirStruct;
        length++;
    }
    sortDirStruct(dirStructArray,length);
    
    int j = 0;
    int largestDigit = 0;
    int largestFileDigit = 0;

    while(j<length){
         largestDigit = inodeDigitCounter(directory,dirStructArray[j].d_name,largestDigit);
         largestFileDigit = fileSizeDigitCounter(directory, dirStructArray[j].d_name, largestFileDigit);
         j++;
    }
    
    int i = 0;
    while(i < length){
        char* name = dirStructArray[i].d_name;
        if (strcmp(name,".") != 0 && strcmp(name,"..") != 0 && checkHiddenFile(dirStructArray[i].d_name)){
            universalPrint(directory,name,ilR_Array,largestDigit, largestFileDigit);
        }
        i++;
    }
    closedir(dir);
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

void printDate(time_t date) {
char dateArr[19];
strftime(dateArr, 19, "%b %e %Y %H:%M", localtime(&date));
printf("%s ",dateArr);
}

void printMode(mode_t mode){
    //help recieved for printing mode from
    //https://courses.engr.illinois.edu/cs241/sp2014/lecture/appendix_L05.pdf
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

void printFile(char* File,bool ilR_Array[],int padding){
    struct stat fileInfo;
    
    if ( getFileInfo(File, &fileInfo) != 0) {
        perror("Error getting file information.\n");
    }

    if (ilR_Array[0]){
        printf("%*.llu ",padding,(unsigned long long) fileInfo.st_ino);
    }
    if (ilR_Array[1]){
        printMode(fileInfo.st_mode);
        printf("%ld ", (long) fileInfo.st_nlink);
        getAndPrintUserName(fileInfo.st_uid);
        getAndPrintGroup(fileInfo.st_gid);
        printf("%5li ", fileInfo.st_size); // we need to fix this padding
        printDate(fileInfo.st_mtime);
    }

    if (hasInvalidChar(File)) {
        printf("'%s'", File);
    } else {
        printf("%s", File);
    }  
    printf("\n");
}

int inodeDigitCounter(char* path,char* name,int largestNum){
    if (strcmp(name,".") == 0 || strcmp(name,"..") == 0){
            return largestNum;
    }
    char *temp = (char *)malloc(1024 * sizeof(char));
    strcpy(temp,path);
    strcat(temp,"/");
    strcat(temp,name);
    struct stat fileInfo;
    getFileInfo(temp,&fileInfo);
    unsigned long long inodeNum = (unsigned long long) fileInfo.st_ino;
    int count = 0;
     do
    {
        count++;
        inodeNum /= 10;

    } while (inodeNum != 0);
    free(temp);

    if(count >= largestNum){
        return count;
    }
    else{
        return largestNum;
    }
}

int fileSizeDigitCounter(char* path,char* name,int largestNum){
    
    if (strcmp(name,".") == 0 || strcmp(name,"..") == 0 || checkHiddenFile(name) == 0){
            return largestNum;
    }
    char *temp = (char *)malloc(1024 * sizeof(char));
    strcpy(temp,path);
    strcat(temp,"/");
    strcat(temp,name);
    struct stat fileInfo;
    getFileInfo(temp,&fileInfo);
    unsigned long long Bytesize = (unsigned long long) fileInfo.st_size;
    int count = 0;
    do
    {
        count++;
        Bytesize /= 10;

    } while (Bytesize != 0);
    free(temp);

    if(count >= largestNum){
        return count;
    }
    else{
        return largestNum;
    }
}
