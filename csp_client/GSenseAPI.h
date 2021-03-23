#ifndef _GSENSEAPI_H_
#define _GSENSEAPI_H_

#include <stdint.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_udp.h>
#include <csp/csp_types.h>

#include "./src/csp_qfifo.h"
#include "config.h"

// Communicatiesysteem
#define REFRESH_ID 0
#define DOWNLOAD_ID 1
#define SET_ID 2
#define GET_ID 3
#define SIZE_ID 4

//Max bytes sent by commands
//  (1 byte for SET_ID + max bytes for ID + 4 bytes for max value) * amount of possible parameters to set
#define MAX_SET_BYTES (1 + MAX_INDEX_SIZE + 4)*JSON_AMOUNT_OF_PARAMETERS
//  (1 byte for get GET_ID + max bytes for ID)* amount of possible parameters to get
#define MAX_GET_BYTES 1+MAX_INDEX_SIZE*JSON_AMOUNT_OF_PARAMETERS
#define MAX_DOWNLOAD_BYTES 1;
#define MAX_REFRESH_BYTES 1;

//json object list sizes
#define JSON_MAX_SIZE 1024
#define JSON_OBJ_LIST_SIZE 100
#define JSON_AMOUNT_OF_PARAMETERS 6
#define MAX_DESCRIPTION_SIZE 50
#define MAX_TYPE_SIZE 10

#define ID_INDEX 0
#define DESC_INDEX 1
#define TYPE_INDEX 2
#define SIZE_INDEX 3
#define VALUE_INDEX 4
#define UPDATED_INDEX 5

#define MAX_INDEX_SIZE 1

// Global variables
uint8_t * bytesToSend;
uint8_t * idArray;

// get_parameter functies
void download_list();
int load_list();
void checkRefresh(uint8_t refresh);

// Handige functies die we overal gebruiken
const char* get_type(uint8_t par_id);
csp_iface_t init_udp();

// Server communication
void sendToServer(uint8_t * data, int length, int amountOfIds);
void returnedFromServer(csp_packet_t * packet, int amountOfIds);

#endif // _GSENSEAPI_H_
