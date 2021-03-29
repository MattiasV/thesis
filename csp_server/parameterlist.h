/*
 * parameterlist1.h
 *
 *  Created on: Mar 24, 2021
 *      Author: jango
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "GSenseHal.h"

enum datatypes {none,u8,i8,u16,i16,u32,i32,f32,c,s,unk32,b};

typedef struct {
  uint8_t id;
  const char * description;
  enum datatypes datatype;
	uint32_t offset; //defined in GSenseHal.h
	int32_t value;
	bool updated;
} parameter_t;

const parameter_t parameterlist[] = {
		{60,"VersionNr", u8	, VERSION_NR,0,0},
		{1,"FirmwareD", u8	,0,0,0},
		{2,"LedCtrl", u8	,	LED_CTRL,0,0},
		{3,"CamCont"	,u32	,	CAM_CTRL,0,0},
		{4,"FloatTest"	,f32	,	FLOAT_TEST,0,0}
};

#endif
