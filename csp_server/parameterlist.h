/*
 * parameterlist1.h
 *
 *  Created on: Mar 24, 2021
 *      Author: jango
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "GSenseHal.h"

typedef struct {
  int id;
  const char * description;
  enum {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7} datatype;
  int size;
	int offset; //defined in GSenseHal.h
} parameter_t;

const parameter_t parameterlist[] = {
		{0,"VersionNr", u8	,1, VERSION_NR },
		{1,"FirmwareD", u8	,1,	},
		{2,"LedCtrl", u8	,1,	LED_CTRL},
		{3,"CamCont"	,u32	,4,	CAM_CTRL},
		{4,"FloatTest"	,f32	,4,	FLOAT_TEST}
};

#endif
