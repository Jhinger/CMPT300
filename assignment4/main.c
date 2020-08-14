#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include "information.h"
#include "search.h"

int fileInodeDigitCounter(char* name, int largestNum){
    struct stat fileInfo;
    getFileInfo(name,&fileInfo);
    unsigned long long inodeNum = (unsigned long long) fileInfo.st_ino;
    int count = 0;
     do
    {
        count++;
        inodeNum /= 10;

    } while(inodeNum != 0);

    if(count >= largestNum){
        return count;
    }
    else{
        return largestNum;
    }
}

int sortArgArray(char * array[],int startIndex,int length){
    int largestDigit = 0;
    for(int i = startIndex; i < length-1; i++){
        largestDigit = fileInodeDigitCounter(array[i],largestDigit);
        int index = i;
        for(int j = i+1; j <length; j++){
            char* minName = array[index];
            char* currentName = array[j];
            if(strcmp(currentName,minName)<= 0){
                index = j;
            }
        }
        char* temp = array[i];
        array[i] = array[index];
        array[index] = temp;
    }
    return largestDigit;
}

void checkIllegalFilePassed(int numArgs, int indexOfFirstFile, char ** args) {
    for (int i = indexOfFirstFile; i < numArgs; i++) {
        struct stat fileInfo;
        int errorCheck = getFileInfo(args[i], &fileInfo);
        if (errorCheck == -1) {
            printf("myls: cannot access '%s': No such file or directory\n", args[i]);
            exit(0);
        }
    }
}

void checkIllegalOptionPassed(char * name) {
    int length = strlen(name);
    for (int i = 1; i < length; i++) {
        if (name[i] == 'i' || name[i] == 'l' || name[i] == 'R') {
            continue;
        }
        printf("myls: invalid option -- '%c'\n", name[i]);
        exit(0);
    }
}


int main(int numArgs, char * args[]) {
    int directoryCount = 0;
    int indexOfFirstFile = 1;
    bool ilR_Array[3] = {false,false,false};

    while(indexOfFirstFile < numArgs && args[indexOfFirstFile][0] == '-' ){
        checkIllegalOptionPassed(args[indexOfFirstFile]);
        if(strchr(args[indexOfFirstFile], 'i'))
            ilR_Array[0]=true;
        if(strchr(args[indexOfFirstFile], 'l'))
            ilR_Array[1]=true;
        if(strchr(args[indexOfFirstFile], 'R'))
            ilR_Array[2]=true;
        indexOfFirstFile++;
    }

    //Checks if a file that does not exist was passed through.
    checkIllegalFilePassed(numArgs, indexOfFirstFile, args);

    int padding = sortArgArray(args,indexOfFirstFile, numArgs);

    if (padding >= 21)
    padding = 21; 
    bool RBool = ilR_Array[2];

    if (numArgs == indexOfFirstFile) {        //No files passed.
        if (RBool == true) {                  //depthSearch
            depthSearch(".", ilR_Array);      //Handles all options.
        }

        else {
            getDirectoryFiles(".", ilR_Array); //Handles only -i and -l options, as well as its combinations.
        }   
    }

    if (numArgs >= indexOfFirstFile + 1 ){   //We have options passed in.
        int index = indexOfFirstFile;

        while (index < numArgs){            //Print sorted files first.
                DIR *dir;
                if ((dir = opendir (args[index])) == NULL) {    //Only printing a file. 
                    printFile(args[index], ilR_Array, padding);
                    directoryCount++;
                } else {
                    directoryCount++;
                }
                index++;
                closedir(dir);
        }
        printf("\n");
        index = indexOfFirstFile;

        while (index < numArgs){            //Sorted directories printed second.
                DIR *dir;
                if ((dir = opendir (args[index])) != NULL) {        //Only printing a file. 
                     if (RBool == true){                             //depthSearch
                        depthSearch(args[index], ilR_Array);    //Handles all options and there combinations.
                        printf("\n");
                    }

                    else {
                        if (directoryCount > 1) {
                            printf("%s:\n", args[index]);
                        }
                        getDirectoryFiles(args[index],ilR_Array); //Handles -i -l and its combinations.
                        printf("\n");
                    }    
                }                                   
                index++;
                closedir(dir);
        }
    }
} 