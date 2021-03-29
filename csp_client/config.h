#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

#define MY_ADDRESS  1     // Address of local CSP node
#define IP "10.0.2.15"      // Address of server

//if you add another datatype, add this to the get_byte_data_in_json case as well with the correct logic
enum datatypes {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7,c=8,s=9,unk32=10,b = 11};

union{
  uint8_t u8bytes[4];
  int8_t i8bytes[4];
  uint16_t u16bytes[2];
  int16_t i16bytes[2];
  uint32_t u32bytes;
  int32_t i32bytes;
  float fbytes;
}fourBytesUnion;

//keep this synchronized with the server!!!
struct json_parameters_t{
  char * parameter_key;
  uint8_t parameter_type;
} json_parameters[] = {
  {"id", u8},
  {"description", s},
  {"datatype", u8},
  {"offset", u32},
  {"value", unk32},
  {"updated", b}
};

#endif // _CONFIG_H_
