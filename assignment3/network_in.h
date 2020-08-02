#ifndef _NETWORK_IN_H_
#define _NETWORK_IN_H_

int socketDescriptor;

void networkIn_init(long recieverPort);

void networkIn_shutdown(); 

void* receiveThread();

void networkIn_join();

#endif