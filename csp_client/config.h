#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

#define MY_ADDRESS  1     // Address of local CSP node
#define IP "10.0.2.15"      // Address of server

union{
  uint8_t u8bytes[4];
  uint16_t u16bytes[2];
  uint32_t u32bytes;
  float fbytes;
}fourBytesUnion;

typedef struct {
  int id;
  const char * description;
  const char * type;
  int size;
  union{
    int intvalue;
    float floatvalue;
  }value;
  bool updated;
} parameter_t;

#endif // _CONFIG_H_
