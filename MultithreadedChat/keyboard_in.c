#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>		
#include <unistd.h>			
#include "keyboard_in.h"
#include "list.h"
#include "screen_out.h"
#include "network_out.h"
#include "network_in.h"

#define MSG_MAX_LENGTH 1024

static List* sendMessagesList;
static pthread_t keyboardInPID;

static char *message;

//Condition Variables for keyboard
pthread_cond_t cond_SendListEmpty = PTHREAD_COND_INITIALIZER;

//Mutex's for CS.
pthread_mutex_t sem_SendListLock = PTHREAD_MUTEX_INITIALIZER;


void* receiveInput(){
  message = NULL;

  while (1) {    
    //Malloc and get message from stdin.     
    message = (char*) malloc(MSG_MAX_LENGTH * sizeof(char));
    fgets(message, MSG_MAX_LENGTH, stdin);

    //Take control of the lock to append item; unlock and signal afterwards.
    lockKeyboardMutex();
    List_append(sendMessagesList, message);
    unlockKeyboardMutex();
    signalKeyboardCond();
  }
  return NULL;
}

void keyboardIn_init(List* list){
  sendMessagesList = list;
  int threadCheck = pthread_create(
    &keyboardInPID,
    NULL,        
    receiveInput,
    NULL);

  if (threadCheck != 0) {
    printf("Failed to create keyBoardIn thread.\n");
  }
} 

void keyboardIn_shutdown(){      
  //Free the malloc'd message.
  free(message);
  message = NULL;

  //Cancel thread and let client know.
  printf("Keyboard thread shutting down...\n");
  pthread_cancel(keyboardInPID);
}

//Join, lock\unlock, signal and destroy functions.
void keyboardIn_join() {
  pthread_join(keyboardInPID, NULL);
}

void lockKeyboardMutex(){
  pthread_mutex_lock(&sem_SendListLock);
}

void unlockKeyboardMutex(){
  pthread_mutex_unlock(&sem_SendListLock);
}

void waitKeyboardCond(){
   pthread_cond_wait(&cond_SendListEmpty, &sem_SendListLock);
}

void signalKeyboardCond(){
  pthread_cond_signal(&cond_SendListEmpty);
}

void destroyKeyboardMutCon(){
  pthread_mutex_unlock(&sem_SendListLock);
  pthread_mutex_destroy(&sem_SendListLock);
  pthread_cond_destroy(&cond_SendListEmpty);
}
