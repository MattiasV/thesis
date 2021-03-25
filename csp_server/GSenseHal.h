#ifndef _GSENSEHAL_H_
#define _GSENSEHAL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// We convert 0x100 to int example 0x100 = 256
typedef enum{
	VERSION_NR = 0,
	LED_CTRL = 256,
	CAM_CTRL = 512,
	FLOAT_TEST = 400,
}uio_register_t;


#define MMAP_ERROR 0
#define MMAP_NO_ERROR 1
#define MAP_SIZE 0x10000000
int open_uio();

int setRegister_uint8 (uio_register_t parameter, uint8_t value);
uint8_t getRegister_uint8 (uio_register_t parameter);

int setRegister_uint32 (uio_register_t parameter, uint32_t value);
uint32_t getRegister_uint32 (uio_register_t parameter);

int setRegister_float (uio_register_t parameter, float value);
float getRegister_float (uio_register_t parameter);

#endif // _GSENSEHAL_H_
