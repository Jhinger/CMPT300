#ifndef _INFORMATION_H_
#define _INFORMATION_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include<stdbool.h>

int getFileInfo(char * pathName, struct stat * fileInfo);

void printFilesInDirectory(char* directory);

void sortDirStruct(struct dirent array[],int length);

void getDirectoryFiles(char* directory,bool ilR_Array[]);//gets all the files in directory and uses universal print

void getAndPrintGroup(gid_t grpNum);

void getAndPrintUserName(uid_t uid);

void printDate(time_t date);

bool hasInvalidChar(char * name);

void printMode(mode_t mode);

void printInode(char* directory); //prints inodes of a directory

void printFile(char* File,bool ilR_Array[],int padding); //if arg is a file it prints the details

void universalPrint(char* path,char* name, bool ilR_Array[],int padding, int largestFileDigit); //prints out according to whether i l R is set

int inodeDigitCounter(char* path,char* name,int largestNum); 

int fileSizeDigitCounter(char* path,char* name,int largestNum);

#endif