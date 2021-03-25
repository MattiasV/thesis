// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "parameterlist.h"

int main(int argc, char* argv[]) {

  union{
    uint8_t u8bytes[2];
    int8_t i8bytes[2];
    uint16_t u16bytes;
    int16_t i16bytes;
  }bytes;

  bytes.u16bytes = 5000;
  printf("u8bytes[0]: %d\n u8bytes[1]: %d\n", bytes.u8bytes[0], bytes.u8bytes[1]);
  bytes.i16bytes = 5000;
  printf("u8bytes[0]: %d\n u8bytes[1]: %d\n", bytes.u8bytes[0], bytes.u8bytes[1]);

  return 0;
}
