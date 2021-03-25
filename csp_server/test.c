// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {

    uint8_t * test1 = malloc(10);
    test1[0] = 5;
    test1[1] = 6;

    float * test2 = malloc(2);
    test2[0] = 700;

    union{
      uint8_t u8byte[4];
      float fbyte;
      uint32_t u32byte;
    }testunion;

    testunion.fbyte = 700.50;

    memcpy(test1,test2,8);

    for(int i = 0; i < 4; i++){
      printf("byte %d: %d\n",i, testunion.u8byte[i]);
    }

    return 0;
}
