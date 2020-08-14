#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#define _process_ancestors_ 550 // for a 64 bit system

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)

#define ANCESTOR_NAME_LEN 16
struct process_info {
long pid;                       /* Process ID */
char name[ANCESTOR_NAME_LEN];   /* Program name of process */
long state;                     /* Current process state */
long uid;                       /* User ID of process owner */
long nvcsw;                     /* # voluntary context switches */
long nivcsw;                    /* # involuntary context switches */
long num_children;              /* # children process has */
long num_siblings;              /* # sibling process has */
};

int main(int argc, char *argv[])
{
    long numFilled = 0;
    struct process_info userStruct[15];
    long size = 15;

    long result = syscall(_process_ancestors_, &userStruct, size, &numFilled);
    if (result != 0) {
        printf("Syscall failed");
    }
    
    for (int i = 0; i < numFilled; i++) {
        struct process_info structInfo = userStruct[i];
        printf("\n");
        printf("Process ID: %ld \n", structInfo.pid);
        printf("Process Name: %s \n", structInfo.name);
        printf("Process State: %ld \n", structInfo.state);
        printf("Process Owner User ID: %ld \n", structInfo.uid);
        printf("Process Voluntary Context Switches: %ld \n", structInfo.nvcsw);
        printf("Process Involuntary Context Switches: %ld \n", structInfo.nivcsw);
        printf("Process Number of Children: %ld \n", structInfo.num_children);
        printf("Process Number of Siblings: %ld \n", structInfo.num_siblings);
        printf("\n");
    }

    //test for size 0
    long numFilled1 = 0;
    struct process_info userStruct1[15];
    long size1 = 0;
    long result1 = syscall(_process_ancestors_, &userStruct1, size1, &numFilled1);
    CHECK(result1 == -1);
    if(result1 != -1){
        printf("\nTest1 failed !!!\n");
        return 0;
    }

    //testforNull array
    long numFilled2 = 0;
    long size2 = 15;
    long result2 = syscall(_process_ancestors_, NULL, size2, &numFilled2);
    CHECK(result2 == -1);
    if(result2 != -1){
        printf("\nTest2 failed !!!\n");
        return 0;
    }

    //Num filled parameter is NULL
    struct process_info userStruct3[15];
    long size3 = 15;
    long result3 = syscall(_process_ancestors_, &userStruct3, size3, NULL);
    CHECK(result3 == -1);
    if(result3 != -1){
        printf("\nTest3 failed !!!\n");
        return 0;
    }

    //giving list size of 1
    long numFilled4 = 0;
    struct process_info userStruct4[1];
    long size4 = 1;
    long result4 = syscall(_process_ancestors_, &userStruct4, size4, &numFilled4);
    CHECK(result4 == 0);
    CHECK(numFilled4 == 1);
    if(result4 != 0 || numFilled4 != 1){
        printf("\nTest4 failed !!!\n");
        return 0;
    }

    //giving list size of 2
    long numFilled5 = 0;
    struct process_info userStruct5[2];
    long size5 = 2;
    long result5 = syscall(_process_ancestors_, &userStruct5, size5, &numFilled5);
    CHECK(result5 == 0);
    CHECK(numFilled5 == 2);
    if(result5 != 0 || numFilled5 != 2){
        printf("\nTest5 failed !!!\n");
        return 0;
    }

    printf("\n***************************Made it here ? All test pass ************************");
    return 0;
}