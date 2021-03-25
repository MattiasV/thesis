/*
 * parameterlist1.h
 *
 *  Created on: Mar 24, 2021
 *      Author: jango
 */

#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_


	typedef struct {
		char descr[20];
		enum {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7} datatype;
		int size;
	} parameter_t;

	const parameter_t parameterlist[] = {
			{"fpgaversion"	,u32	,1	},
			{"led"			,u8		,1	},
			{"temperature"	,f32	,1	}
	};



#endif
