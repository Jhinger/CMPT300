#include <assert.h>
#include <stdio.h>
#include "list.h"
#include <signal.h>

//For keeping track of the maximum number of heads (lists) allowed.
List maxListHeads[LIST_MAX_NUM_HEADS]; 
Node maxListNodes[LIST_MAX_NUM_NODES];

static int queueHeads[LIST_MAX_NUM_HEADS];  //Queue for list heads.
static int headFront = -1;         //Set to -1 so they can be reset in functions if needed.
static int headBack = -1;
static unsigned int headSize = 0;

static int queueNodes[LIST_MAX_NUM_NODES];  //Queue for list nodes.
static int nodeFront = -1;         //Set to -1 so they can be reset in functions if needed.
static int nodeBack = -1;
static unsigned int nodeSize = 0;

//Circular Queue functions here:
static bool isEmptyHeads() {
    bool returnVal = (headSize == 0 ? true : false);
    return returnVal;
}

static bool isEmptyNodes() {
    bool returnVal = (nodeSize == 0 ? true : false);
    return returnVal;
}

static bool isFullHeads() {
    bool returnVal = (headSize == LIST_MAX_NUM_HEADS ? true : false);
    return returnVal;
}

static bool isFullNodes() {
    bool returnVal = (nodeSize == LIST_MAX_NUM_NODES ? true : false);
    return returnVal;

}

static void enqueueHeads(int index) {               //Do not have to worry about queue getting full because we always dequeue before enqueueing meaning that we always have space for atleast one more.
    if (isFullHeads()) {
        return;
    } else {
        if (headFront == -1) {          //Queue has never been used before.
            headFront = 0;
            headBack = 0;
        }
        queueHeads[headBack] = index;
        headBack = (headBack + 1) % LIST_MAX_NUM_HEADS;
        headSize++;
    }
}

static void enqueueNodes(int index) {
    if (isFullNodes()) {
        return;
    } else {
        if (nodeFront == -1) {          //Queue has never been used before.
            nodeFront = 0;
            nodeBack = 0;
        }
        queueNodes[nodeBack] = index;
        nodeBack = (nodeBack + 1) % LIST_MAX_NUM_NODES;
        nodeSize++;
    }
}

static int dequeueHeads() {
  if (isEmptyHeads()) {
    return -1;
  } else {
    int returnIndex = queueHeads[headFront];
    headFront = (headFront + 1) % LIST_MAX_NUM_HEADS;
    headSize--;
    return returnIndex;
  }
}

static int dequeueNodes() {
  if (isEmptyNodes()) {
    return -1;
  } else {
    int returnIndex = queueNodes[nodeFront];
    nodeFront = (nodeFront + 1) % LIST_MAX_NUM_NODES;
    nodeSize--;
    return returnIndex;
  }
}


//List functions here:

static void initializeArrays() {
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++) {  //List Heads array.
        maxListHeads[i].head = NULL;     
        maxListHeads[i].tail = NULL;
        maxListHeads[i].currentItem = NULL;
        maxListHeads[i].listIndex = i;
        maxListHeads[i].nodeCount = 0;
        maxListHeads[i].beforeList = false;
        maxListHeads[i].beyondList = false;
        enqueueHeads(i);
    }

    for (int j = 0; j < LIST_MAX_NUM_NODES; j++) {  //List Nodes array.
        maxListNodes[j].next = NULL;
        maxListNodes[j].prev = NULL;
        maxListNodes[j].item = NULL;
        maxListNodes[j].nodeIndex = j;
        enqueueNodes(j);
    }
}

List * List_create() {
    static int initializeCheck = 0;
    if (!initializeCheck) {                         //Checking whether this is first call, if it is, do O(N) setup.
        initializeArrays();
        initializeCheck = 1;
    }

    int nextHeadIndex = dequeueHeads();             //Getting Index of next available Head spot.
    if (nextHeadIndex == -1) {
        return NULL;
    }
    return &(maxListHeads[nextHeadIndex]);
}

int List_count(List * pList) {                      //Returns the number of nodes in use in pList.
    assert(pList != NULL);
    return pList -> nodeCount;
}

void * List_first(List * pList) {
    if (pList -> nodeCount == 0) {                  //Checking if the list is empty.
        pList -> currentItem = NULL;
        return NULL;
    }
     
    pList -> beforeList = false;
    pList -> beyondList = false;
    pList -> currentItem = pList -> head;           //Setting the current item to the head.
    return pList -> currentItem -> item;                   //returning the pointer to the first item.
}

void * List_last(List * pList) {
    if (pList -> nodeCount == 0) {                  //Checking if the list is empty.
        pList -> currentItem = NULL;
        return NULL;
    }

    pList -> beyondList = false;
    pList -> beforeList = false;
    pList -> currentItem = pList -> tail;           //Setting the current item to the tail.
    return pList -> tail -> item;                              //returning the pointer to the last item.
}

void * List_next(List * pList) {
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {
        return NULL;
    }
    if (pList -> nodeCount == 1) {
        if (pList -> beforeList == true) {
            pList -> beforeList = false;
            return pList -> currentItem -> item;
        }
        if (pList -> beyondList == false && pList -> beforeList == false) {
            pList -> beyondList = true;
            return NULL;
        }
        if (pList -> beyondList == true) {
            return NULL;
        }
    }
    if (pList -> currentItem == pList -> tail) {    //If current is on tail and is about to be pushed beyond the list.
        pList -> beyondList = true;
        return NULL;
    }
    if (pList -> beyondList == true) {    //If current is aleady beyond the list.
        return NULL;
    } 
    if (pList -> beforeList == true) {   //If current is beforeList, reset the boolean value, and return the head value since we do not move current if it was before list.
        pList -> beforeList = false;     //We do not move currentItem to next node as currentItem was before the list, therefore we have a "ghost" move.
        return pList -> currentItem -> item;
    } 

    pList -> currentItem = pList -> currentItem -> next;
    return pList -> currentItem -> item;
}

void * List_prev(List * pList) {
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {
        return NULL;
    }
    if (pList -> nodeCount == 1) {
        if (pList -> beforeList == true) {
            return NULL;
        }
        if (pList -> beyondList == false && pList -> beforeList == false) {
            pList -> beforeList = true;
            return NULL;
        }
        if (pList -> beyondList == true) {
            pList -> beyondList = false;
            return pList -> currentItem -> item;
        }
    }
    if (pList -> beforeList == true) {    //If current is already before the list.
        return NULL;
    } 
    if (pList -> currentItem == pList -> head) {
        pList -> beforeList = true;
        return NULL;
    }                                                   
    if (pList -> beyondList == true) {       //If current is beyondList, reset the boolean value and return the tail value since we do not move current if it was beyond the list.
        pList -> beyondList = false;         //We do not move currentItem to the prev node as currentItem was beyond the list, therefore we have a "ghost" move.
        return pList -> currentItem -> item;
    }
    pList -> currentItem = pList -> currentItem -> prev;
    return pList -> currentItem -> item; 
}

void * List_curr(List * pList) {        //returns a pointer to the current item in pList.
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {
        return NULL;
    }
    if (pList -> beforeList == true || pList -> beyondList == true) { //If currentItem is beyond or before the list, return NULL.
        return NULL;
    } else {                        //Otherwise, if current is in  a valid range, return its item.
        return pList -> currentItem -> item;
    }
}

int List_add(List * pList, void * pItem) {
    int nextNodeIndex = dequeueNodes();
    if (nextNodeIndex == -1) {          //If there are no more nodes to use (All 100 nodes are in use).
        return -1;
    } else {
        if (pList -> nodeCount == 0) {                          //If this is the first item being added to the list.
            maxListNodes[nextNodeIndex].item = pItem;           //Set item.
            maxListNodes[nextNodeIndex].next = NULL;
            maxListNodes[nextNodeIndex].prev = NULL;
            pList -> head = &(maxListNodes[nextNodeIndex]);
            pList -> tail = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem = &(maxListNodes[nextNodeIndex]);
            pList -> nodeCount += 1;
            return 0;
        } else if (pList -> beforeList == true) {        //If current is before the list.
            maxListNodes[nextNodeIndex].item = pItem;           //Set item.
            maxListNodes[nextNodeIndex].next = pList -> currentItem;
            maxListNodes[nextNodeIndex].prev = NULL;
            pList -> beforeList = false;
            pList -> currentItem -> prev = &(maxListNodes[nextNodeIndex]);
            pList -> head = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem = &(maxListNodes[nextNodeIndex]); //Sets current item to the newly added item.
            pList -> nodeCount += 1;
            return 0;
        } else if (pList -> beyondList == true) {        //If current is beyond the list.
            maxListNodes[nextNodeIndex].item = pItem;           //Set item.
            maxListNodes[nextNodeIndex].prev = pList -> currentItem;
            maxListNodes[nextNodeIndex].next = NULL;
            pList -> beyondList = false;
            pList -> tail = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem -> next = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem = &(maxListNodes[nextNodeIndex]);
            pList -> nodeCount += 1;
            return 0;
        } else if (pList -> currentItem == pList -> tail) {     //If current is on the tail.
            maxListNodes[nextNodeIndex].item = pItem;           //Set item.
            maxListNodes[nextNodeIndex].prev = pList -> currentItem;
            maxListNodes[nextNodeIndex].next = NULL;
            pList -> currentItem -> next = &(maxListNodes[nextNodeIndex]);
            pList -> tail = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem = &(maxListNodes[nextNodeIndex]);
            pList -> nodeCount += 1;
            return 0;
        } else {                                                //Otherwise, current is somewhere other than before or beyond the list.
            maxListNodes[nextNodeIndex].item = pItem;           //Set item.
            maxListNodes[nextNodeIndex].next = pList -> currentItem -> next;
            maxListNodes[nextNodeIndex].prev = pList -> currentItem;
            pList -> currentItem -> next -> prev = &(maxListNodes[nextNodeIndex]);
            pList -> currentItem -> next = &(maxListNodes[nextNodeIndex]);      //Setting the prev/next for the nodes.
            pList -> nodeCount += 1;                            //Incrementing the number of nodes in the list.
            pList -> currentItem = &(maxListNodes[nextNodeIndex]);  //Setting current pointer to the new node.
            return 0;
        }
    }
}

int List_insert(List * pList, void * item) {    //Don't have to check for empty nodes as add function does it. Don't want to accidentally dequeue twice.
    if (pList -> beyondList == true || pList -> beforeList == true) { //If the current is beyond or before the list, the functionality does not change compared to List_add(), therefore we call it directly.
        return List_add(pList, item);   //In these three cases, just regularly add the item.
    } else {                            //Otherwise, we back up the current pointer by one and then call List_add().
        List_prev(pList);
        return List_add(pList, item);
    }
}

int List_append(List * pList, void * item) {    //Adds to the end of the list.
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {      //If there is nothing in the list, call add normally.
        return List_add(pList, item);   //Returns 0 on success, -1 on failure.
    } else {
        pList -> currentItem = pList -> tail;   //Move the current pointer back one position.
        return List_add(pList, item);           //Returns 0 on success, -1 on failure.
    }
}

int List_prepend(List * pList, void * item) {   //Adds to the beginning of the list.
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {              //If there is nothing in the list, call add normally.
        return List_add(pList, item);
    } else {
        pList -> currentItem = pList -> head;
        List_prev(pList);
        return List_add(pList, item);
    }
}

void * List_remove(List * pList) {  //Removes node from list, returns item stored in removed node.
    assert(pList != NULL);
    if (pList -> nodeCount == 0) {  //List is empty.
        return NULL;
    } 
    
    if (pList -> beforeList == true || pList -> beyondList == true) {
        return NULL;
    }

    if (pList -> nodeCount == 1) {   //Only one node in the list.
        pList -> head = NULL;
        pList -> tail = NULL;
        pList -> nodeCount -= 1;            //Decrement the number of nodes in the list.
        int index = pList -> currentItem -> nodeIndex;
        enqueueNodes(index);                //Requeueing the node Index so it can be used again by another list.
        pList -> currentItem = NULL;
        return maxListNodes[index].item;    //Returning the item of the removed node.
    }

    if (pList -> currentItem == pList -> head) {  //Current pointer is on the head.
        int index = pList -> currentItem -> nodeIndex;
        pList -> head = pList -> head -> next;          //Set head to be the next node.
        pList -> currentItem = pList -> head -> prev;
        pList -> head -> prev = NULL;
        pList -> currentItem -> next = NULL;
        pList -> currentItem = pList -> head;
        pList -> nodeCount -= 1;                       //Decrement the number of nodes in the list.
        enqueueNodes(index);                //Adding the index back into pool to be reused.
        return maxListNodes[index].item;   //Return the item of the removed node.
    }

    if (pList -> currentItem == pList -> tail) {  //Current pointer is on the tail.
        int index = pList -> currentItem -> nodeIndex;
        pList -> tail = pList -> tail -> prev;
        pList -> tail -> next = NULL;
        pList -> currentItem -> prev = NULL;
        pList -> currentItem = pList -> tail;
        pList -> beyondList = true;         //RIGHT HERE --------------------
        pList -> nodeCount -= 1;
        enqueueNodes(index);
        return maxListNodes[index].item;

    } else {                                //Current Pointer is somewhere in the middle.
        int index = pList -> currentItem -> nodeIndex;
        pList -> currentItem -> prev -> next = pList -> currentItem -> next;
        pList -> currentItem -> next -> prev = pList -> currentItem -> prev;
        pList -> currentItem -> next = NULL;
        pList -> currentItem -> prev = NULL;
        pList -> currentItem = &(maxListNodes[index + 1]);
        enqueueNodes(index);
        pList -> nodeCount -= 1;
        return maxListNodes[index].item;
    }
}

void List_concat(List * pList1, List * pList2) {
    //If both lists are empty
    if (pList1 -> nodeCount == 0 && pList2 -> nodeCount == 0) {
        int index = pList2 -> listIndex;
        enqueueHeads(index);
        pList2 -> beforeList = false;
        pList2 -> beyondList = false;
        pList2 -> currentItem = NULL;
        pList2 -> head = NULL;
        pList2 -> nodeCount = 0;
        pList2 -> tail = NULL;
        return;
    }
    if (pList1 -> nodeCount == 100) {
        int index = pList2 -> listIndex;
        enqueueHeads(index);
        pList2 -> beforeList = false;
        pList2 -> beyondList = false;
        pList2 -> currentItem = NULL;
        pList2 -> head = NULL;
        pList2 -> nodeCount = 0;
        pList2 -> tail = NULL;
        return;
    }
    //If list 1 is empty but list two is not.
    if (pList1 -> nodeCount == 0 && pList2 -> nodeCount > 0) {
        pList1 -> head = pList2 -> head;
        pList1 -> tail = pList2 -> tail;
        pList1 -> nodeCount = pList2 -> nodeCount;
        pList1 -> currentItem = pList1 -> head;     //Based on Piazza answer from instructors, List1 current pointer should be before the list after concat.
        pList1 -> beforeList = true;
        int listIndex = pList2 -> listIndex;
        enqueueHeads(listIndex);
        pList2 -> beforeList = false;
        pList2 -> beyondList = false;
        pList2 -> currentItem = NULL;
        pList2 -> head = NULL;
        pList2 -> nodeCount = 0;
        pList2 -> tail = NULL;
        return;
    }
    //If list 2 is empty but list 1 is not.
    if (pList1 -> nodeCount > 0 && pList2 -> nodeCount == 0) {
        int index = pList2 -> listIndex;
        enqueueHeads(index);
        return;
    }
    //If neither list is empty.
    if (pList1 -> nodeCount > 0 && pList2 -> nodeCount > 0) {
        //Check if current is beyond list, if it is, after concat current should still be beyond the end.
        bool outOfBounds = false;
        if (pList1 -> beyondList == true) {
            outOfBounds = true;
        }
        pList1 -> tail -> next = pList2 -> head;
        pList2 -> head -> prev = pList1 -> tail;
        pList1 -> tail = pList2 -> tail;
        pList1 -> nodeCount += pList2 -> nodeCount;
        int listIndex = pList2 -> listIndex;
        enqueueHeads(listIndex);
        pList2 -> beforeList = false;
        pList2 -> beyondList = false;
        pList2 -> currentItem = NULL;
        pList2 -> head = NULL;
        pList2 -> nodeCount = 0;
        pList2 -> tail = NULL;
        if (outOfBounds) {  
            pList1 -> currentItem = pList1 -> tail;         //Sets current to the tail of the new concat'd list.
            pList1 -> beyondList = true;     //Sets current to remain beyond the concat'd list. 
        }
        return;
    }
}

void List_free(List * pList, FREE_FN pItemFreeFn) {
    if(pList == NULL) return;
    if (pList -> nodeCount == 0) {
        pList -> currentItem = NULL;
        pList -> head = NULL;
        pList -> tail = NULL;
        pList -> nodeCount = 0;
        pList -> beforeList = false;
        pList -> beyondList = false;
        int requeueIndex = pList -> listIndex;
        enqueueHeads(requeueIndex); //Requeue the index of the head so it can be used by another list.
        pList = NULL;
        return;
    }
    if (pList -> nodeCount == 1) {  //If there is exactly one node in the list.
        pList -> beforeList = false;
        pList -> beyondList = false;
        pList -> nodeCount = 0;
        (*pItemFreeFn)(pList -> head -> item);
        int nodeRequeue = pList -> head -> nodeIndex;
        enqueueNodes(nodeRequeue);
        pList -> head = NULL;
        pList -> tail = NULL;
        int headRequeue = pList -> listIndex;
        enqueueHeads(headRequeue);
        pList -> currentItem = NULL;
        pList = NULL;
        return;
    }                               //If there is more than one node in the list.
    pList -> beforeList = false;
    pList -> beyondList = false;
    pList -> currentItem = pList -> head;
    while (pList -> currentItem != pList -> tail) {
        int requeueNode = pList -> currentItem -> nodeIndex;
        (*pItemFreeFn)(pList -> currentItem -> item);
        pList -> currentItem -> prev = NULL;
        pList -> currentItem = pList -> currentItem -> next;
        pList -> currentItem -> prev -> next = NULL;
        enqueueNodes(requeueNode);
    }
    (*pItemFreeFn)(pList -> currentItem -> item);
    pList -> currentItem -> next = NULL;
    pList -> currentItem -> prev = NULL;
    int requeueTail = pList -> currentItem -> nodeIndex;
    enqueueNodes(requeueTail);
    pList -> head = NULL;
    pList -> currentItem = NULL;
    pList -> tail = NULL;
    pList -> nodeCount = 0;
    int requeueHead = pList -> listIndex;
    pList = NULL;
    enqueueHeads(requeueHead);
    return;
}

void * List_trim(List * pList) {
    if (pList -> nodeCount == 0) {
        return NULL;
    }
    if (pList -> nodeCount == 1) {  //If there is only one item in the list.
        int index = pList -> tail -> nodeIndex;
        enqueueNodes(index);
        pList -> head = NULL;
        pList -> tail = NULL;
        pList -> nodeCount -= 1;
        pList -> currentItem = NULL;
        pList -> beyondList = false;
        pList -> beforeList = false;
        return maxListNodes[index].item;
    } 
    //More than one node in the list.
    pList -> beyondList = false;
    pList -> beforeList = false;
    pList -> currentItem = pList -> tail;
    pList -> tail = pList -> tail -> prev;
    int index = pList -> currentItem -> nodeIndex;
    enqueueNodes(index);
    pList -> tail -> next = NULL;
    pList -> currentItem -> prev = NULL;
    pList -> currentItem = pList -> tail;
    pList -> nodeCount -= 1;
    return maxListNodes[index].item;    //return the item stored in the last node.
}

void * List_search(List * pList, COMPARATOR_FN pComparator, void * pComparisonArg) {
    assert(pList != NULL);
    if (pList -> beforeList == true) {
        pList -> beforeList = false;
    }
    while (pList -> currentItem != pList -> tail) {    //While current does not reach the end of the list.
        int returnVal = pComparator(pList -> currentItem -> item, pComparisonArg);  //Client code checks for a match, returns 1 on success and 0 on failure.
        if (returnVal == 1) {
            return pList -> currentItem -> item;
        } else {
            pList -> currentItem = pList -> currentItem -> next;
        }
    }
    //If current has reached beyond the list.
    int returnVal = pComparator(pList -> currentItem -> item, pComparisonArg);  //Client code checks for a match, returns 1 on success and 0 on failure.
    if (returnVal == 1) {
        return pList -> currentItem -> item;
    } else {
        pList -> beyondList = true;
    } 
    return NULL;
}


