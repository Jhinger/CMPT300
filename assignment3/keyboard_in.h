#ifndef _KEYBOARD_IN_H_
#define _KEYBOARD_IN_H_

#include "list.h"

void keyboardIn_init(List* list);

void keyboardIn_shutdown(); 

void* receiveInput();

void lockKeyboardMutex();

void unlockKeyboardMutex();

void waitKeyboardCond();

void signalKeyboardCond();

void destroyKeyboardMutCon();

void keyboardIn_join();

#endif