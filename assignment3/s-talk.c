#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>	
#include "list.h"


int main(int argc, char** argv) {
    List * keyboardSendList = List_create();    //Shared list between the keyboard-send threads.
    List * receiveScreenList = List_create();   //Shared list between the receive-screen threads.

    int myPort = atoi(argv[1]);
    int remotePort = atoi(argv[3]);
    char * remoteName = malloc(sizeof(argv[1]));
    remoteName = argv[2];

    printf("Remote name is: %s \n Local port is: %d\n", remoteName, myPort);

    struct sockaddr_in socket_fd;
	memset(&socket_fd, 0, sizeof(socket_fd));
	socket_fd.sin_family = AF_INET;                   // Connection may be from network
	socket_fd.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	socket_fd.sin_port = htons(myPort);               // Host to Network short

    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    bind (socketDescriptor, (struct sockaddr*) &socket_fd, sizeof(socket_fd));

    
}