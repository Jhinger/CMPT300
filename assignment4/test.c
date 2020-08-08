#include <stdio.h>

int main(int numArgs, char * args[]) {
    for (int i = 0; i < numArgs; i++) {
        printf("%s\n", args[i]);
    }
}