#ifndef _NETWORK_OUT_H_
#define _NETWORK_OUT_H_

void networkOut_init(char* destP,long ourP, char* name);

void networkOut_shutdown(); 

void* sendThread();

void networkOut_join();

#endif