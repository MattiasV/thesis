#ifndef _PARAMETERSERVICE_H_
#define _PARAMETERSERVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_udp.h>

#include "GSenseHal.h"
#include "./src/csp_qfifo.h"
#include "parameterlist.h"

// Parameter variable
#define MY_ADDRESS  1			// Address of local CSP node
#define IP "0.0.0.0"      // Not important for server, IP gets updated to client IP each time it receives from a client
#define MAX_PACKET_SIZE 512   //512 Bytes is the max packet size.
#define TIMEOUT 1000

// Global variables
static csp_iface_t iface;
int refresh;

// Msg systeem
#define REFRESH_ID 0
#define DOWNLOAD_ID 1
#define SET_ID 2
#define GET_ID 3
#define SIZE_ID 4

#define MAX_DESCRIPTION_SIZE 50
#define MAX_INDEX_SIZE 1
#define MAX_SET_BYTES_REQUEST (1 + MAX_INDEX_SIZE + 4)*sizeof(parameterlist)/sizeof(parameter_t)

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
  int id;
  const char * description;
  const char * type;
  int size;
  union{
    int intvalue;
    float floatvalue;
  }value;
  bool updated;
} parameter_list;

// functies
void convert_parameter_list(uint8_t list_in_bytes[]);
void listen_in();
void send_refresh();
void send_parameter_list();
int check_datatype_and_put_in_byte_array(uint8_t * list_in_bytes, int datatype, int list_in_bytes_index, int parameterlist_index);
void set_parameter(uint8_t * msg, int length);
void get_parameter(uint8_t * data, int length);
int check_type_and_set_register(int index, uint8_t * data, int type, int offset);
int check_type_and_get_register(int datatype, int offset);
#endif // _PARAMETERSERVICE_H_
