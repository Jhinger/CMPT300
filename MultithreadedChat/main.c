#include <stdio.h>
#include <stdlib.h>
#include "network_in.h"
#include "network_out.h"
#include "keyboard_in.h"
#include "screen_out.h"

int main(int argc, char** args)
{
    printf("Starting s-talk application...\n");

    long recieverPort = atol(args[1]);
    char* hostName = args[2];
    char* destPort = args[3];

    //Startup threads in their modules.
    networkIn_init(recieverPort);
    networkOut_init(destPort, recieverPort, hostName);
    

    keyboardIn_join();
    screenOut_join();
    networkIn_join();
    networkOut_join();
    return 0;
}