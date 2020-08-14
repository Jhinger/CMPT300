#include <linux/kernel.h>
#include <linux/syscalls.h>

struct array_stats_s {
long min;
long max;
long sum;
};
SYSCALL_DEFINE3(
        array_stats, /* syscall name */
        struct array_stats_s*, stats, /* where to write stats */
        long*, data, /* data to process */
        long, size) /* # values in data */
{
    long maxElement = 0;
    long minElement = 0;
    long sumCounter = 0;
    long currentElement = 0;
    long copyOverCheck = 0;
    int index = 1;

    //Verifying size parameter.
    if (size <= 0) return -EINVAL;

    //Checking that items are being copied over properly. 
    copyOverCheck = copy_from_user(&currentElement, &data[0], sizeof(long));
    if (copyOverCheck != 0) return -EFAULT;
    copyOverCheck = copy_from_user(&maxElement, &data[0], sizeof(long));
    if (copyOverCheck != 0) return -EFAULT;
    copyOverCheck = copy_from_user(&minElement, &data[0], sizeof(long));
    if (copyOverCheck != 0) return -EFAULT;
    copyOverCheck = copy_from_user(&sumCounter, &data[0], sizeof(long));
    if (copyOverCheck != 0) return -EFAULT;
    
    //Loop to find max, min, and sum statistics.
    for (index; index < size; index++) {
        copyOverCheck = copy_from_user(&currentElement, &data[index], sizeof(long));
        if (copyOverCheck != 0) return -EFAULT;

        if (currentElement < minElement) {
            minElement = currentElement;
        }

        if (currentElement > maxElement) {
            maxElement = currentElement;
        }
        sumCounter += currentElement;
    }
 
   // sets min element 
   if(copy_to_user(&stats->min,&minElement,sizeof(stats->min))){
        return -EFAULT; //if results in inaccessible memory
    }
    
    // sets max element
    if(copy_to_user(&stats->max,&maxElement,sizeof(stats->max))){
        return -EFAULT; //if results in inaccessible memory
    }
   
   // sets sum
   if(copy_to_user(&stats->sum,&sumCounter,sizeof(stats->sum))){
        return -EFAULT; //if results in inaccessible memory
    }

    return 0;
}