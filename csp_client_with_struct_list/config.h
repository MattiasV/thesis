#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

//UDP things
#define MY_ADDRESS 11
#define DEST_IP "0.0.0.0"
#define DEST_ADDR 6
#define DEST_PORT 10
#define TIMEOUT 1000

union{
  uint8_t u8bytes[4];
  int8_t i8bytes[4];
  uint16_t u16bytes[2];
  int16_t i16bytes[2];
  uint32_t u32bytes;
  int32_t i32bytes;
  float fbytes;
}fourBytesUnion;

#endif // _CONFIG_H_
