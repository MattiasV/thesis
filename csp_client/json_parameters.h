#ifndef _JSON_PARAMETERS_H_
#define _JSON_PARAMETERS_H_

#include "config.h"

//keep this synchronized with the server!!!
struct {
  char * parameter_key;
  uint8_t parameter_type;
} json_parameter_list[] = {
  {"id", u8},
  {"description", s},
  {"datatype", u8},
  {"offset", u32},
  {"value", unk32},
  {"updated", b}
};

#endif //_JSON_PARAMETERS_H_
