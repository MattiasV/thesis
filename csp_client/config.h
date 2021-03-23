#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

#define MY_ADDRESS  1     // Address of local CSP node
#define IP "0.0.0.0"      // Address of server

union{
  uint8_t u8bytes[4];
  uint16_t u16bytes[2];
  uint32_t u32bytes;
  float fbytes;
}fourBytesUnion;

#endif // _CONFIG_H_
