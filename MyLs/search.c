#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include "information.h"
#include<stdbool.h>

//Recursively sorts and prints all files in the path that is specified.
void depthSearch(char * path, bool ilR_Array[]) {
    struct dirent* currentDir;
	struct dirent ** directoryArray;
	char* folderArray[1024];
	int folderCount = 0;

	//Alphasort is built in - organizes it into directoryArray.
	int numFiles = scandir(path, &directoryArray, 0, alphasort);

	//No need to return error message - just return.
	if (numFiles <= 0){
		return;
	}

	//Loop through directoryArray - Recurse if folder, otherwise it continues in the loop.
	//Print path name first.
	printf("%s:\n", path);
	//inder changes
    int padding = 0;
	int largestFileDigit = 0;
	if(ilR_Array[0] == true){
		for(int j=0; j < numFiles;j++){
			currentDir = directoryArray[j];
			largestFileDigit = fileSizeDigitCounter(path, currentDir -> d_name, largestFileDigit);
			padding = inodeDigitCounter(path,currentDir->d_name,padding);
		}
	}

	for (int i = 0; i < numFiles; i++)
	{	
		currentDir = directoryArray[i];

		//If currentDir is a directory and not a hidden file.
		if (currentDir -> d_type == DT_DIR && checkHiddenFile(currentDir->d_name)) {
			char * nextPath = (char *) malloc(MAX_NAME_LEN);
			if (path[strlen(path) - 1] == '/') {
				snprintf(nextPath, MAX_NAME_LEN, "%s%s", path, currentDir -> d_name);
			} else {
				snprintf(nextPath, MAX_NAME_LEN, "%s/%s", path, currentDir -> d_name);
			}
			folderArray[folderCount] = nextPath;
			folderCount++;
			
			//Call printing file information here.
            		universalPrint(path,currentDir -> d_name, ilR_Array,padding, largestFileDigit);
		}

		//Otherwise, if the file is not hidden.
		else if (checkHiddenFile(currentDir->d_name)) {
			char nextPath[MAX_NAME_LEN];
			snprintf(nextPath, MAX_NAME_LEN, "%s/%s", path, currentDir->d_name);
			
			//Call printing file information here.
			universalPrint(path,currentDir -> d_name, ilR_Array,padding, largestFileDigit);  
		}
		free(currentDir);
	}

	//Recursively call depthSearch on only the folders.
	for (int p = 0; p < folderCount; p++){
		printf("\n");
		depthSearch(folderArray[p],ilR_Array);
	}
	freeArrays(folderArray, directoryArray, folderCount);
}

//Returns false if hidden file so that it cannot pass in depthSearch.
int checkHiddenFile(char * name) {
    if (*name == '.') {
        return 0;
    }
    return 1;
}

void freeArrays(char ** folderArray, struct dirent ** directoryArray, int folderCount) {
	for (int j = 0; j < folderCount; j++) {
		free(folderArray[j]);
    }
	free(directoryArray);
}
