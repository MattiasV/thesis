#ifndef _JSON_PARAMETERS_H_
#define _JSON_PARAMETERS_H_

#include "config.h"

//keep this synchronized with the server!!!

//if you add another datatype, add this to the get_byte_data_in_json case as well with the correct logic
enum datatypes {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7,c=8,s=9,dtype=10,b=11};

struct {
  char * parameter_key;
  uint8_t parameter_type;
} json_parameter_list[] = {
  {"id", u8},
  {"description", s},
  {"datatype", u8},
  {"offset", u32},
  {"value", dtype},
  {"updated", b}
};

#endif //_JSON_PARAMETERS_H_
