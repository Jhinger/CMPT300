#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>			
#include <unistd.h>			
#include "screen_out.h"
#include "list.h"
#include "keyboard_in.h"
#include "network_out.h"
#include "network_in.h"

static List* recieveMessageList;
static pthread_t screenOutPID;

pthread_cond_t cond_receiveListEmpty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sem_receiveListLock = PTHREAD_MUTEX_INITIALIZER;

void* screenOutThread() {
    while (1) {
        int count = List_count(recieveMessageList);
        if (count == 0) {
            lockScreenOutMutex();
            waitScreenOutCond();
            unlockScreenOutMutex();
        }
       
        //Take control of the lock.
        lockScreenOutMutex();
        List_first(recieveMessageList);
        char* message = List_remove(recieveMessageList);

        size_t len = strlen(message) - 1;
        if (message[0] == '!' && len == 1) {
            unlockScreenOutMutex();
            destroyScreenMutCon();
            free(message);
            networkOut_shutdown();
            networkIn_shutdown(); 		        
	}
        printf("\nRemote User: ");
        fflush(stdout);
        fputs(message, stdout);
        free(message);
    
        //Unlock the NetworkIn thread can continue.
        unlockScreenOutMutex();
    }
    return NULL;
}

//Initialize the screenOut thread.
void screenOut_init(List* list){
    recieveMessageList = list;
      int threadCheck = pthread_create(
                        &screenOutPID,
                        NULL,
                        screenOutThread,
                        NULL);
	
    if (threadCheck != 0) {
        printf("Failed to create networkOut thread.\n");
    }
}

//Shutdown the screenOut thread.
void screenOut_shutdown(){ 
      printf("Screen out thread shutting down\n");
      pthread_cancel(screenOutPID);
}

void screenOut_join() {
    pthread_join(screenOutPID, NULL);
}

void lockScreenOutMutex(){
    pthread_mutex_lock(&sem_receiveListLock);
}

void unlockScreenOutMutex(){
    pthread_mutex_unlock(&sem_receiveListLock);
}

void waitScreenOutCond(){
    pthread_cond_wait(&cond_receiveListEmpty, &sem_receiveListLock);
}

void signalScreenOutCond(){
     pthread_cond_signal(&cond_receiveListEmpty);
}

void destroyScreenMutCon(){
    pthread_mutex_unlock(&sem_receiveListLock);
    pthread_mutex_destroy(&sem_receiveListLock);
    pthread_cond_destroy(&cond_receiveListEmpty);
}
