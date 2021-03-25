// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "parameterlist.h"

int main(int argc, char* argv[]) {

    printf("sizeof parameterlist: %ld\n", sizeof(parameterlist)/sizeof(parameter_t));

    return 0;
}
