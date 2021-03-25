#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

#define MY_ADDRESS  1     // Address of local CSP node
#define IP "10.0.2.15"      // Address of server

union{
  uint8_t u8bytes[4];
  int8_t i8bytes[4];
  uint16_t u16bytes[2];
  int16_t i16bytes[2];
  uint32_t u32bytes;
  int32_t i32bytes;
  float fbytes;
}fourBytesUnion;

typedef struct {
  uint8_t id;
  const char * description;
  enum {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7} datatype;
  uint32_t offset;
  union{
    int32_t intvalue;
    float floatvalue;
  }value;
  bool updated;
} parameter_t;

#endif // _CONFIG_H_
