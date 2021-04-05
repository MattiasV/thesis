/*
 * parameterlist1.h
 *
 *  Created on: Mar 24, 2021
 *      Author: jango
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include "GSenseHal.h"

enum datatypes {none,u8,i8,u16,i16,u32,i32,f32,c,s,b};

typedef struct {
  uint8_t id;
  char description[15];
  enum datatypes datatype;
	uint32_t offset; //defined in GSenseHal.h
	int32_t value;
	bool updated;
} parameter_t;

parameter_t parameterlist[] = {
		{10,"VersionNr", u8, VERSION_NR, 20, 0},
		{1,"FirmwareD", u8, 0, 0, 0},
		{2,"LedCtrl", u8,	LED_CTRL, 0, 0},
		{3,"CamCont", u32,	CAM_CTRL, 0, 0},
		{4,"Test", i32,	TEST, 0, 0}
};

#define DIFFERENT_PARAMETERS sizeof(parameterlist)/sizeof(parameter_t)

union{
  uint8_t par_list_bytes[DIFFERENT_PARAMETERS*sizeof(parameter_t)];
  parameter_t par_list[DIFFERENT_PARAMETERS];
}parameter_list_union;


#endif
