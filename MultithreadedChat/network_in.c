#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			
#include <unistd.h>			
#include <pthread.h>
#include <signal.h>
#include "network_in.h"
#include "network_out.h"
#include "list.h"
#include "screen_out.h"

#define MSG_MAX_LEN 1024

static long PORT;
static pthread_t networkInPID;
static List* recieveMessageList;
static char *messageRx;
static int errorCheck;
struct sockaddr_in sinRemote;
  
//Receive thread that receives packet from remote address.  
void* receiveThread() {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                   
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    
    sin.sin_port = htons(PORT);                  
    
    // Create the socket descriptor.
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0); 
    if (socketDescriptor == -1) {
        printf("Error creating socket.\n");
    }
        
    //Bind and set message to NULL.    
    errorCheck = bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    if (errorCheck == -1) {
        printf("Error binding socket.\n");
    }

    while (1) {
        unsigned int sin_len = sizeof(sinRemote);
        messageRx = (char*) malloc(MSG_MAX_LEN * sizeof(char));
        errorCheck = recvfrom( 
                  socketDescriptor,
                  messageRx, 
                  MSG_MAX_LEN, 
                  0,
                  (struct sockaddr *) &sinRemote, 
                  &sin_len
                );

        if (errorCheck == -1) {
          printf("Error receiving in Network In thread.\n");
        }

        //Take control of the lock and append item so screen thread can also access it.
        lockScreenOutMutex();
        List_append(recieveMessageList, messageRx);
        unlockScreenOutMutex();
        signalScreenOutCond();  
    }
    return NULL;
}

//Initialize the NetworkIn module.
void networkIn_init(long recieverPort){
  recieveMessageList = List_create();  
  screenOut_init(recieveMessageList);
  PORT = recieverPort;
  int threadCheck = pthread_create(
                    &networkInPID,
                    NULL,
                    receiveThread,
                    NULL);

  if (threadCheck != 0) {
    printf("Failed to create networkIN thread.\n");
  }       
}

//Shutdown the NetworkIn module.
void networkIn_shutdown(){
    sleep(1);
    free(messageRx);
    messageRx = NULL;
    close(socketDescriptor);
    printf("Network in thread shutting down...\n");
    pthread_cancel(networkInPID);
    screenOut_shutdown();
}

void networkIn_join(){
  pthread_join(networkInPID, NULL);
}
