#ifndef _JSON_FUNCTIONS_H_
#define _JSON_FUNCTIONS_H_

#include <stdint.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define JSON_FLAG JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY

//global variables
struct json_object * parsed_json;

void store_list_from_bytes(uint8_t * data, int length);
int get_byte_data_in_json(json_object * jparameter, uint8_t * data, int index_of_data);
void add_values(uint8_t * data, int length);
void set_value_in_jobject(json_object * jparameter, uint8_t * data, int * index_of_data);
void set_updated(int amountOfIds);
void print_list();
json_object * get_json_from_file();
int get_type(uint8_t par_id);

#endif // _JSON_FUNCTIONS_H_
