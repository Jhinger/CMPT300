#ifndef _SCREEN_OUT_H_
#define _SCREEN_OUT_H_

#include "list.h"

void screenOut_init(List* list);

void screenOut_shutdown(); 

void* screenOutThread();

void lockScreenOutMutex();

void unlockScreenOutMutex();

void waitScreenOutCond();

void signalScreenOutCond();

void destroyScreenMutCon();

void screenOut_join();

#endif