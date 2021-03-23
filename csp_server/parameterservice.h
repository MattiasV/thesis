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


// Parameter variable
#define MY_ADDRESS  1			// Address of local CSP node
#define IP "0.0.0.0"      // Not important for server, IP gets updated to client IP each time it receives from a client
#define MAX_PACKET_SIZE 512   //512 Bytes is the max packet size.

// Global variables
static csp_iface_t iface;
int refresh;
//ALTIJD EINDIGEN MET EEN KOMMA
char parameter_list[] = {"0,VersionNr,uint8_t,4,0,0,1,FirmwareD,uint8_t,4,0,0,2,LedCtrl,uint8_t,1,0,0,3,CamCont,uint32_t,1,0,0,4,FloatTest,float,4,0,0,"}; // id,description,type,size,value,id,description,type,size,value,...
#define LIST_SIZE sizeof(parameter_list);

uint8_t parameter_list_byte[sizeof(parameter_list)];

// Msg systeem
#define REFRESH_ID 0
#define DOWNLOAD_ID 1
#define SET_ID 2
#define GET_ID 3
#define SIZE_ID 4

// functies
void initialize_parameters();
void listen_in();
void send_refresh();
void send_parameter_list();
void set_parameter(uint8_t msg[]);
void get_parameter(char * data);
int get_type(uint8_t);

#endif // _PARAMETERSERVICE_H_
