#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

#define ANCESTOR_NAME_LEN 16
struct process_info {
        long pid; /* Process ID */
        char name[ANCESTOR_NAME_LEN]; /* Program name of process */
        long state; /* Current process state */
        long uid; /* User ID of process owner */
        long nvcsw; /* # voluntary context switches */
        long nivcsw; /* # involuntary context switches */
        long num_children; /* # children process has */
        long num_siblings; /* # sibling process has */
};

SYSCALL_DEFINE3(
        process_ancestors, /* syscall name for macro */
        struct process_info*, info_array, /* array of process info strct */
        long, size, /* size of the array */
        long*, num_filled) /* # elements written to array */
{
    long numFilledCounter = 0;
    long numChildrenCounter = 0;
    long numSiblingsCounter = 0;
    long indexCounter = 0;
    struct task_struct * currentProcess = current;
    struct list_head * listHead;                //For sibling, child counters.
    struct process_info processInfo;            //Struct to store the process info locally.

    if (size <= 0) return -EINVAL;              //Parameter checking.
    if (info_array == NULL) return -EFAULT;
    if (num_filled == NULL) return -EFAULT;

    while (numFilledCounter != size) {
        
        processInfo.pid = (long) currentProcess -> pid;                 //Getting the process ID, casted to long otherwise volatile error or something.
        processInfo.state = (long) currentProcess -> state;             //Getting process state, casted to long.
        processInfo.uid = (long) currentProcess -> cred -> uid.val;     //Getting the User ID of the process owner.
        processInfo.nvcsw = currentProcess -> nvcsw;                    //Getting the voluntary context switches.
        processInfo.nivcsw = currentProcess -> nivcsw;                  //Getting the involuntary context switches.

        /* #define list_for_each(pos, head) \
	        for (pos = (head)->next; pos != (head); pos = pos->next)    //list_for_each definition as seen in list.h from https://elixir.bootlin.com/linux/latest/source/include/linux/list.h#L552
        */                                                              //list_for_each does not count the head (extra).

        list_for_each(listHead, &(currentProcess -> children)) {
            numChildrenCounter++;
        }
        list_for_each(listHead, &(currentProcess -> sibling)) { 
            numSiblingsCounter++;
        }

        strncpy(processInfo.name, currentProcess -> comm, 15);          //Copying the name of the process, only allowed maximum 16 characters so we have to cut it off at index 15.
                                                                        //if(copy_to_user(&localName, currentProcess -> comm, 15)) return -EFAULT;
                                                                        //strncpy(processInfo.name, localName, 15);
                                                                        //Code above does not give error but gives weird characters when outputted.

        processInfo.num_children = numChildrenCounter;
        processInfo.num_siblings = numSiblingsCounter;

        if (copy_to_user(&(info_array[indexCounter]), &(processInfo), sizeof(struct process_info))) return -EFAULT; //Copying the local struct we created into appropriate array index.

        indexCounter++;                                 //Increment the array index.
        numFilledCounter++;                             //Increment the number of array_info indices we've written to.
        if (currentProcess->parent == currentProcess){  //If we are at the swapper process, break out of the loop.
            break;                                      
        }
        currentProcess = currentProcess -> parent;      //Move the current process up the tree.

        numChildrenCounter = 0;                         //Reset the Child and Sibling Counters so values are not being reused for each process.
        numSiblingsCounter = 0;
    }

        
    if (copy_to_user(num_filled, &(numFilledCounter), sizeof(long))) return -EFAULT;    //Copying over the number of struct indices we filled.

    return 0;
}



