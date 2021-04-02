#ifndef _JSON_PARAMETERS_H_
#define _JSON_PARAMETERS_H_

#include "config.h"

//keep this synchronized with the server!!!

enum datatypes {none,u8,i8,u16,i16,u32,i32,f32,c,s,b};

typedef struct {
  uint8_t id;
  const char * description;
  enum datatypes datatype;
	uint32_t offset; //defined in GSenseHal.h
	int32_t value;
	bool updated;
} parameter_t;

#define DIFFERENT_PARAMETERS 5

union{
  uint8_t par_list_bytes[DIFFERENT_PARAMETERS*sizeof(parameter_t)];
  Memory par_list[DIFFERENT_PARAMETERS];
}memory_list_union;


#endif //_JSON_PARAMETERS_H_
