#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include "information.h"

//Finds all the subdirectories in the currentDirectory.
directoryInformation findDirectories(char * currentDirectory) {
    directoryInformation info;
    DIR * dir;
    struct dirent * directory;
    dir = opendir(currentDirectory);

    info.size = 0;
    while((directory = readdir(dir)) != NULL){ 
        //If it's not a directory.
        if (directory -> d_type != DT_DIR) {
            continue;
        }

        //If it's hidden file, ignore.
        if (strcmp(directory->d_name, ".") != 0 && strcmp(directory->d_name, "..") == 0) {
            continue;
        }

        info.subdirectories[info.size] = *directory;
        info.size++;
    }
    closedir(dir);
    return info;
}


//Recursively sorts and prints.
void depthSearch(char * path) {
    struct dirent* dp;
	struct stat buf;
	struct dirent ** entryList;
	char* dirList[1024];
	int dirNum = 0;

	// Scan the dir and sort it lexicographically using alphasort
	int count = scandir(path, &entryList, 0, alphasort);

	// Scan dir failure 
	if (count <= 0){
		printf("dfs_print scandir failure\n");
		exit(1);
	}

	// Loop through all entries in the corresponding dir path
	printf("%s:\n", path);
	for(int k = 0; k < count; k++)
	{	
		dp = entryList[k];

		char temp[1024];
    	if(path[strlen(path)-1] == '/') {
        	snprintf(temp, sizeof(temp), "%s%s", path, dp->d_name);
        } else {
        	snprintf(temp, sizeof(temp), "%s/%s", path, dp->d_name);
        }

		int error = stat(temp, &buf);

		// check stat failure
		if (error < 0){
			printf("dfs_print static error");
			exit(1);
		}

		// if dp is a directory and is not hiden file, "." or ".." 
		if (dp -> d_type == DT_DIR && checkHiddenFile(dp->d_name)){
			char* newPath = (char*) malloc(1024*sizeof(char));
			snprintf(newPath, 1024, "%s/%s", path, dp->d_name);
			dirList[dirNum] = newPath;
			dirNum++;
			//print_info_base_on_path(newPath, dp, i, l);
            printf("%s\n", dp -> d_name);
		}

		// if dp is file and is not hiden file, "." or ".." 
		else if (checkHiddenFile(dp->d_name)){
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			//print_info_base_on_path(newPath, dp, i, l);
            printf("%s\n", dp -> d_name);
		}

		free(dp);
	}

	// recursive call
	for (int p = 0; p < dirNum; p++){
		printf("\n");
		depthSearch(dirList[p]);
	}

	// clean up
	for (int q = 0; q < dirNum; q++) {
		free(dirList[q]);
	    free(entryList);
    }
}

//Returns false if hidden file so that it cannot pass in depthSearch.
int checkHiddenFile(char * name) {
    if (*name == '.') {
        return 0;
    }
    return 1;
}

int main(int numArgs, char ** args) {
    depthSearch(args[1]);
}