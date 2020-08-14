#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <signal.h>

#include "network_out.h"
#include "network_in.h"
#include "keyboard_in.h"
#include "screen_out.h"

#define MAX_MSG_LENGTH 1024

static pthread_t networkOutPID;
static char* destPORT;
static long ourPORT;
static char* hostName;
static List* sendMessagesList;

struct addrinfo hints, *remoteInfo;
static int errorCheck;
static int count;

void * sendThread() {

    //Setting up hints struct for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    //Getting remote address info into remoteInfo struct, and some error checking.
    errorCheck = 0;
    if ((errorCheck = getaddrinfo(hostName, destPORT, &hints, &remoteInfo)) != 0) {
        perror("Error getting address information of Remote.\n");
        exit(EXIT_FAILURE);
    }

    while(1){

        count = List_count(sendMessagesList);

        //While the buffer is empty (Nothing to send), block until we get something.
        if (count == 0) {
            lockKeyboardMutex();
            waitKeyboardCond();
            unlockKeyboardMutex();
        }

        //Take control of the mutex for the CS.
        lockKeyboardMutex();

        List_first(sendMessagesList);
        char* message = List_remove(sendMessagesList);
            
        //Send the item, and some error checking.
        if (sendto(socketDescriptor, message, MAX_MSG_LENGTH, 0, remoteInfo -> ai_addr, remoteInfo -> ai_addrlen) == -1) {
            printf("Error sending message in Network Out.\n");
        }

        unlockKeyboardMutex();

        //Determine whether we are aborting or not.
        size_t len = strlen(message) - 1;
        if (message[0] == '!' && len == 1) {
            free(message);
            destroyKeyboardMutCon();
            networkIn_shutdown();
            networkOut_shutdown(); 
        }

        free(message);
    } 
    return NULL;
}

//Initialize the NetworkOut module.
void networkOut_init(char* destP,long ourP, char* name){
    
    sendMessagesList = List_create();
    keyboardIn_init(sendMessagesList);
    
    destPORT = destP;
    ourPORT = ourP;
    hostName = name;
  
    int threadCheck =  pthread_create(
        &networkOutPID,
        NULL,
        sendThread,
        NULL);

    if (threadCheck != 0) {
        printf("Failed to create networkOut thread.\n");
    }
}

//Shutdown the NetworkOut module.
void networkOut_shutdown() {
    //We are allowed to sleep in shutdown to allow lists to clear.
    sleep(1);
    freeaddrinfo(remoteInfo);
    keyboardIn_shutdown();
     
    close(socketDescriptor);

    pthread_cancel(networkOutPID); 
    printf("Network out thread shutting down...\n");
}

void networkOut_join(){
    pthread_join(networkOutPID, NULL);
}


