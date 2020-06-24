#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#define _array_stats_ 549 // for a 64 bit system

struct array_stats_s {
long min;
long max;
long sum;
};

int main(int argc, char *argv[])
{
    struct array_stats_s test = {0,0,0};
    
    long array[10] = {1,2,3,4,5,6,7,8,9,10};

    printf("\nDiving to kernel level\n");
    int result = syscall(_array_stats_, &test, array, 10);
    printf("\nRising to user level w/ result = %d\n\n", result);

    printf("User space min is %ld\n", test.min); 
    printf("User space max is %ld\n", test.max);
    printf("User space sum is %ld\n", test.sum);

    struct array_stats_s test1 = {0,0,0};
    long array1[10] = {4,3,6,3,22,55,-32,-54,-4,0};
    int result1 = syscall(_array_stats_, &test1, array1, 10);

    if(result1 !=0 || test1.min != -54 || test1.max != 55 || test1.sum != 3) {
        printf("test1 failed");
    }

    //testing with max value at the start
    struct array_stats_s test2 = {0,0,0};
    long array2[10] = {55,3,6,3,22,4,-32,-54,-4,0};
    int result2 = syscall(_array_stats_, &test2, array2, 10);

    if(result2 !=0 || test2.min != -54 || test2.max != 55 || test2.sum != 3) {
        printf("test2 failed\n");
        return -1;
    }

    int result3 = syscall(_array_stats_, &test2, array2, 0);
    // should return -1 as size is  0
    if(result3 != -1){
        printf("test3 failed\n");
        return -1;
    }

    //give array of size 1
    struct array_stats_s test4 = {0,0,0};
    long array4[1] = {-3};
    int result4 = syscall(_array_stats_, &test4, array4, 1);
    if(result4 !=0 || test4.min != -3 || test4.max != -3 || test4.sum != -3) {
        printf("test4 failed\n");
        return -1;
    }

    //giving null pointer as struct
    int result5 = syscall(_array_stats_, NULL, array2, 11);
    if(result5 != -1){
        printf("test5 failed\n");
    }

    //give array of size 2
    struct array_stats_s test6 = {0,0,0};
    long array6[2] = {-3,-2};
    int result6= syscall(_array_stats_, &test6, array6, 2);
    if(result6 !=0 || test6.min != -3 || test6.max != -2 || test6.sum != -5){
        printf("test6 failed\n");
        return -1;
    }

    //testing all values same
    struct array_stats_s test7 = {0,0,0};
    long array7[5] = {3, 3, 3, 3, 3};
    int result7 = syscall(_array_stats_, &test7, array7, 5);
    if (result7 != 0 || test7.min != 3 || test7.max != 3 || test7.sum != 15) {
        printf("test7 failed\n");
    }

    printf("Made it here!!! All tests passed...\n\n");
    return 0;
}