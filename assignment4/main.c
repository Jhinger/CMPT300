#include <stdio.h>
#include <string.h>
#include "information.h"

int main(int numArgs, char * args[]) {

    if(numArgs == 1){//means we are just doing ls to current directory
        printf("\nbehaving like normal ls\n");
        printFilesInDirectory(".");
    }

    if(numArgs == 2 ){
        if(strcmp(args[1],"-l")== 0){//means we are just doing ls -l on current directory
            longListingCurr(".");
        }

        else if(strcmp(args[1],"-i")== 0){//means we are just doing ls -i on current directory
            printInode(".");
        }

        else if(strcmp(args[1],"-r")== 0){//means we are just doing ls -r on current directory
            
        }

        else{// maybe check for star here
            printFilesInDirectory(args[1]); //means we are doing ls on a certain directory eg ls ..
        }

    }

    if(numArgs == 3 ){
        if(strcmp(args[1],"-l")== 0){//means we are just doing ls -l on certain directory
            longListingCurr(args[2]);
        }

        else if(strcmp(args[1],"-i")== 0){//means we are just doing ls -i on certain directory
            printInode(args[2]);
        }

        else if(strcmp(args[1],"-R")== 0){//means we are just doing ls -r on certain directory
            
        }
    }
} 