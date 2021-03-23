#ifndef _JSONFUNCTIONS_H_
#define _JSONFUNCTIONS_H_

#include <stdint.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"

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

//global variables
struct json_object * parsed_json;

void store_list(uint8_t * data);
uint8_t * getDataString(int * id, char description[20], char datatype[20], int * size, int * value, bool * updated, uint8_t * data);
int getValuesFromJson(struct json_object * json_object_list[JSON_OBJ_LIST_SIZE]);
void addValues(uint8_t * data, int length);
void setUpdated(int amountOfIds);
void print_list();


#endif // _JSONFUNCTIONS_H_
