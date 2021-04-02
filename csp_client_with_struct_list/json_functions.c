#include "json_functions.h"
#include "json_parameters.h"
#include "GSenseAPI.h"
#include "config.h"


void store_list_from_bytes(uint8_t * data, int length)
{

	struct json_object * jobj;
	struct json_object * jobjarray;
	struct json_object * jparameter;

	jobj = json_object_new_object();
	jobjarray = json_object_new_array();
	for(int i = 0; i < length; i++){
		parameter_list_union.par_list_bytes[i] = data[i];
	}

	jparameter = json_object_new_object();
	int index = i;
	index = get_byte_data_in_json(jparameter, parameter_list_union.par_list, length/sizeof(parameter_t));
	i = index;
	json_object_array_add(jobjarray, jparameter);
	json_object_object_add(jobj, "parameters", jobjarray);
	json_object_to_file_ext("parameters.json", jobj, JSON_FLAG);
}


int get_par_list_in_json(json_object * jparameter, parameter_t * parameterlist, int size){

	for(int i = 0; i < size; i++){
		//set in json to be coded
	}

}


void add_values(uint8_t * data, int length){

	struct json_object * jobj;
	jobj = json_object_new_object();
	json_object * jobj_array = get_json_from_file();
	int json_array_size = json_object_array_length(jobj_array);

	for(int i = 0; i < length;){
		for(int j = 0; j < json_array_size; j++){
			json_object * jparameter = json_object_array_get_idx(jobj_array, j);
			int id = json_object_get_int(json_object_object_get(jparameter,"id"));
			if(data[i] == id) {
				i++;
				int index = i;
				index = set_value_in_jobject(jparameter, data, i);
				i = index;
			}
		}
	}
	json_object_object_add(jobj, "parameters", jobj_array);
	json_object_to_file_ext("parameters.json", jobj, JSON_FLAG);
	json_object_put(parsed_json);
}


int set_value_in_jobject(json_object * jparameter, uint8_t * data, int index_of_data){

	printf("index_of_data: %d\n", index_of_data);
	json_object * jdatatype = json_object_object_get(jparameter, "datatype");
	json_object * jvalue = json_object_object_get(jparameter, "value");
	int datatype = json_object_get_int(jdatatype);
	printf("datatype: %d\n", datatype);
	switch(datatype){

		case u8:
			json_object_set_int(jvalue,data[index_of_data++]);
			break;

		case i8:
			json_object_set_int(jvalue,data[index_of_data++]);
			break;

		case u16:
			fourBytesUnion.u8bytes[0] = data[index_of_data++];
			fourBytesUnion.u8bytes[1] = data[index_of_data++];
			json_object_set_int(jvalue,fourBytesUnion.u16bytes[0]);
			break;

		case i16:
			fourBytesUnion.i8bytes[0] = data[index_of_data++];
			fourBytesUnion.i8bytes[1] = data[index_of_data++];
				json_object_set_int(jvalue,fourBytesUnion.i16bytes[0]);
			break;

		case u32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[index_of_data++];
			}
				json_object_set_int(jvalue,fourBytesUnion.u32bytes);
			break;

		case i32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.i8bytes[i] = data[index_of_data++];
			}
			json_object_set_int(jvalue,fourBytesUnion.i32bytes);
			break;

		case f32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[index_of_data++];
			}
			printf("fbytes: %f\n", fourBytesUnion.fbytes);
			json_object_set_int(jvalue,fourBytesUnion.fbytes);
			break;
	}
	return index_of_data;
}


void set_updated(int amountOfIds){

	struct json_object * jobj;
	jobj = json_object_new_object();
	struct json_object * jobj_array = get_json_from_file();
	int size = json_object_array_length(jobj_array);

	for(int i = 0; i < size; i++){
		json_object * jparameter = json_object_array_get_idx(jobj_array, i);
		for(int j = 0; j < amountOfIds; j++){
			int id = json_object_get_int(json_object_object_get(jparameter,"id"));
			if(idArray[j] == id) {
				json_object_set_boolean(json_object_object_get(jparameter,"updated"),true);
			}
		}
	}

	json_object_object_add(jobj, "parameters", jobj_array);
	json_object_to_file_ext("parameters.json", jobj, JSON_FLAG);
	json_object_put(parsed_json);

}


void print_list()
{

	struct json_object * jobj;
	jobj = json_object_new_object();
	struct json_object * jobj_array = get_json_from_file(); //getting the amount of different id's
	json_object_object_add(jobj, "parameters", jobj_array);
	printf("%s", json_object_to_json_string_ext(jobj, JSON_FLAG));
	json_object_put(parsed_json);

}


struct json_object * get_json_from_file(){ // open json file, read all the json_objects and put them in the json_object pointer lists

	char buffer[JSON_MAX_SIZE];
	int array_size;
	int succes;
	struct json_object *parameter_array;

	FILE *fptr;
	// File openen en naar buffer
	fptr = fopen("parameters.json" , "r");
	fread(buffer,JSON_MAX_SIZE,1,fptr);
	fclose(fptr);
	// De json gaat een array van structs zijn
	parsed_json = json_tokener_parse(buffer);
	succes = json_object_object_get_ex(parsed_json, "parameters" , &parameter_array);

	if(succes == 0)
	{
		printf("Failed to open json file with array tag \"parameters\". \n");
		return NULL;
	}
	return parameter_array;
}


int get_type(uint8_t par_id)
{

	int type = 0;
	int found = 0;

	json_object * jobj = get_json_from_file();
	int size = json_object_array_length(jobj);
	for(int i = 0; i < size; i++){
		json_object * jparameter = json_object_array_get_idx(jobj,i);
		int search_id;
		search_id = json_object_get_int(json_object_object_get(jparameter,"id"));
		if(par_id == search_id)
		{
			type = json_object_get_int(json_object_object_get(jparameter, "datatype"));
		}
	}
	json_object_put(parsed_json);
	return type;
}
