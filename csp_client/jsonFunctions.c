#include "jsonFunctions.h"
#include "GSenseAPI.h"
#include "config.h"

void store_list(uint8_t * data, int length)
{
	// JSON file gaan openen
	int num;
	FILE *fptr;

	fptr = fopen("parameters.json" , "w+");
	fprintf(fptr,"{ \n");
	fprintf(fptr,"\"parameters\": [{ \n");

	// De parameters gaan invullen
	int index_of_data = 0;
	for(int i = 0; i < JSON_OBJ_LIST_SIZE; i++){

		parameter_t parameterlist[JSON_OBJ_LIST_SIZE];
		get_data_in_parameterlist(&parameterlist[i], data, int length, &index_of_data);

		// ALLe info in JSON zetten, bij de laatste geen komma op het einde!!
		fprintf(fptr, "\t \t \"id\":\"%d\",\n", parameterlist[i].id);
		fprintf(fptr, "\t \t \"description\":\"%s\",\n", parameterlist[i].description);
		fprintf(fptr, "\t \t \"datatype\":\"%s\",\n", parameterlist[i].datatype);
		frptinf(fptr, "\t \t \"offset\":\"%d\",\n", parameterlist[i].offset);
		fprintf(fptr, "\t \t \"value\":\"%d\",\n", parameterlist[i].value);
		fprintf(fptr, "\t \t \"updated\":\"%d\"\n", parameterlist[i].updated);

		if(data != NULL)
			fprintf(fptr, "\t }, { \n");
	}

	fprintf(fptr,"}] \n");
	fprintf(fptr, "}\n");

	// De file sluiten
	fclose(fptr);

}

uint8_t * get_data_in_parameterlist(parameter_t * parameter, uint8_t * data, int length, int * index_of_data){

	for(*index_of_data; *index_of_data < length;){

		parameter->id = data[*index_of_data++]; //set parameter id
		for(int i = 0; data[*index_of_data] != '\0'; i++){
			parameter->description[i] = data[*index_of_data++]; // fill parameter->description char
		}
		parameter->datatype = data[*indexofdata++]; //set parameter datatype
		for(int i = 0; i < 4; i++){
			fourBytesUnion.u8bytes[i] = data[*index_of_data++];
		}
		parameter->offset = fourBytesUnion.u32bytes; //set parameter offset
		for(int i = 0; i < 4; i++){
			fourBytesUnion.i8bytes[i] = data[*index_of_data++];
		}
		if(parameter->datatype != f32)
			parameter->value.intvalue = fourBytesUnion.i32bytes; //set parameter value if not float
		else
			parameter->value.floatvalue = fourBytesUnion.fbytes; //set parameter value if float
		parameter->updated = data[*index_of_data++]; //set parameter updated
	}


}

void addValues(uint8_t * data, int length){

	struct json_object * json_object_list[JSON_OBJ_LIST_SIZE];
	char * serialized_list = malloc(JSON_MAX_SIZE); // this is the list where all the variables will be stored as a string.
	uint8_t * serialized_list_bytes = malloc(JSON_MAX_SIZE); //this will be the serialized list put in a byte array to be send to store_list()
	char * temp_serialized_list = malloc(JSON_OBJ_LIST_SIZE);
	bzero(serialized_list, JSON_MAX_SIZE);
  bzero(serialized_list_bytes, JSON_MAX_SIZE);

	int size = getValuesFromJson(json_object_list);
	parameter_t parameterlist[JSON_OBJ_LIST_SIZE];

	for(int i = 0; i < size; i++){

		parameterlist[i].id = json_object_get_int(json_object_list[i*(size + 1) + ID_INDEX]);
		parameterlist[i].description = json_object_get_string(json_object_list[i*(size + 1) + DESC_INDEX]);
		parameterlist[i].datatype = json_object_get_int(json_object_list[i*(size + 1) + TYPE_INDEX]);
		parameterlist[i].offset = json_object_get_int(json_object_list[i*(size + 1) + OFFSET_INDEX]);
		if(parameterlist[i].id != 4)
			parameterlist[i].value.intvalue = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		else
		parameterlist[i].value.floatvalue = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		parameterlist[i].updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);
	}

	for(int i = 0; i < length;){ // Here we will read the output of the server and look at which ID's and value he sent us
		for(int j = 0; j<size; j++){
			if(data[i] == parameterlist[j].id){
				set_value_in_parameter_struct(&parameterlist[j], data, &i);
			}
		}
	}

	for(int i = 0; i < size; i++){

		bzero(temp_serialized_list, JSON_OBJ_LIST_SIZE);
		int id = parameterlist[i].id;
		const char * description = parameterlist[i].description;
		int datatype = parameterlist[i].datatype;
		int offset = parameterlist[i].offset;
		bool updated = parameterlist[i].updated;
		if(datatype != f32)
			sprintf(temp_serialized_list,"%d,%s,%d,%d,%d,%d,", id, description, datatype, offset, par_list[i].value.intvalue, updated); // puts variables in string
		else
			sprintf(temp_serialized_list,"%d,%s,%d,%d,%f,%d,", id, description, datatype, offset, par_list[i].value.floatvalue, updated); // puts variables in string
		if(i == 0)
			strcpy(serialized_list, temp_serialized_list);
		else
			strcat(serialized_list, temp_serialized_list);
		}


	snprintf(serialized_list_bytes, JSON_MAX_SIZE, "%s", serialized_list);
	printf("serialized_list_bytes: %s\n", serialized_list_bytes);
	store_list(serialized_list_bytes);

	free(temp_serialized_list);
	free(serialized_list);
	free(serialized_list_bytes);

}

void set_value_in_parameter_struct(parameter_t * parameter, uint8_t * data, int * index_of_data){
	switch(parameter->datatype){
		case u8:
			parameter->value.intvalue = data[*index_of_data];
			break;
		case i8:
			parameter->value.intvalue = data[*index_of_data];
			break;
		case u16:
			fourBytesUnion.u8bytes[0] = data[*index_of_data++];
			fourBytesUnion.u8bytes[1] = data[*index_of_data++];
			parameter->value.intvalue = fourBytesUnion.u16bytes;
			break;
		case i16:
		fourBytesUnion.i8bytes[0] = data[*index_of_data++];
			fourBytesUnion.i8bytes[1] = data[*index_of_data++];
			parameter->value.intvalue = fourBytesUnion.i16bytes;
			break;
		case u32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[*index_of_data++];
			}
			parameter->value.intvalue = fourBytesUnion.u32bytes;
		case i32:
		for(int i = 0; i < 4; i++){
			fourBytesUnion.i8bytes[i] = data[*index_of_data++];
		}
		parameter->value.intvalue = fourBytesUnion.i32bytes;
		case f32:
		for(int i = 0; i < 4; i++){
			fourBytesUnion.u8bytes[i] = data[*index_of_data++];
		}
		parameter->value.floatvalue = fourBytesUnion.fbytes;
	}
}

void setUpdated(int amountOfIds){

	struct json_object * json_object_list[JSON_OBJ_LIST_SIZE];
	char * serialized_list = malloc(JSON_MAX_SIZE); // this is the list where all the variables will be stored as a string.
	uint8_t * serialized_list_bytes = malloc(JSON_MAX_SIZE); //this will be the serialized list put in a byte array to be send to store_list()
	char * temp_serialized_list = malloc(JSON_OBJ_LIST_SIZE);

	bzero(json_object_list, JSON_OBJ_LIST_SIZE);
	bzero(serialized_list, JSON_MAX_SIZE);
  bzero(serialized_list_bytes, JSON_MAX_SIZE);

	int size = getValuesFromJson(json_object_list);

	for(int i = 0; i < size; i++){
		bzero(temp_serialized_list, JSON_OBJ_LIST_SIZE);
		int par_id = json_object_get_int(json_object_list[i*(size+1) + ID_INDEX]);
		const char * description = json_object_get_string(json_object_list[i*(size + 1) + DESC_INDEX]);
		int datatype = json_object_get_int(json_object_list[i*(size + 1) + TYPE_INDEX]);
		int offset = json_object_get_int(json_object_list[i*(size + 1) + OFFSET_INDEX]);
		int value = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		bool updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);

		for(int j = 0; j < amountOfIds; j++){
			printf("par_id: %d\t idArray: %d\n", par_id, idArray[j]);
			if(idArray[j] == par_id){
				updated = true;
			}
		}

		sprintf(temp_serialized_list,"%d,%s,%d,%d,%d,%d,", par_id, description, type, offset, value, updated); // puts variables in string

		if(size == 0)
			strcpy(serialized_list, temp_serialized_list);
		else
			strcat(serialized_list, temp_serialized_list);

	}
	snprintf(serialized_list_bytes, JSON_MAX_SIZE, "%s", serialized_list);

	store_list(serialized_list_bytes);

	json_object_put(parsed_json);

	free(idArray);
	free(temp_serialized_list);
	free(serialized_list);
	free(serialized_list_bytes);
}


void print_list()
{

	struct json_object * json_object_list[JSON_OBJ_LIST_SIZE]; // list of pointers pointing to a json_object struct, each json_object struct is a sub of the big json_object struct which represents the .json file

	int size = getValuesFromJson(json_object_list); //getting the amount of different id's

	for(int i = 0; i < size; i++){ //for each id, get the values out of the list of pointers
		int par_id = json_object_get_int(json_object_list[i*(size + 1) + ID_INDEX]);
		const char * description = json_object_get_string(json_object_list[i*(size + 1) + DESC_INDEX]);
		int datatype = json_object_get_int(json_object_list[i*(size + 1) + TYPE_INDEX]);
		int offset = json_object_get_int(json_object_list[i*(size + 1) + OFFSET_INDEX]);
		int value = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		bool updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);

		printf(" %d \t | %s \t | %d \t | %d \t | %d \t | %d \t \n",par_id, description, datatype, offset, value, updated); //printing the list

	}
}

int getValuesFromJson(struct json_object * json_object_list[JSON_OBJ_LIST_SIZE]){ // open json file, read all the json_objects and put them in the json_object pointer lists
	char buffer[JSON_MAX_SIZE];
	int array_size;
	int succes;
	struct json_object *parameter_array;
	struct json_object *parameter;
	struct json_object *id;
	struct json_object *description;
	struct json_object *type;
	struct json_object *offset;
	struct json_object *value;
	struct json_object *updated;
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
	}

	array_size = json_object_array_length(parameter_array);

	for(int i = 0; i < array_size; i++) // get all the different sub objects out of the big json_object, and put them in the json_object pointer list
	{
		// Get values on name
		parameter = json_object_array_get_idx(parameter_array, i);
		json_object_object_get_ex(parameter, "id" , &id);
		json_object_object_get_ex(parameter, "description" , &description);
		json_object_object_get_ex(parameter, "type" , &type);
		json_object_object_get_ex(parameter, "offset" , &offset);
		json_object_object_get_ex(parameter, "value" , &value);
		json_object_object_get_ex(parameter, "updated" , &updated);


		json_object_list[i*(array_size + 1) + ID_INDEX] = id;
		json_object_list[i*(array_size + 1) + DESC_INDEX] = description;
		json_object_list[i*(array_size + 1) + TYPE_INDEX] = type;
		json_object_list[i*(array_size + 1) + OFFSET_INDEX] = offset;
		json_object_list[i*(array_size + 1) + VALUE_INDEX] = value;
		json_object_list[i*(array_size + 1) + UPDATED_INDEX] = updated;
	}

	return array_size;

}

/**
Hier gaan we een datapakket [0] sturen, die simpelweg aan starterkit vraagt of de parameterlijst
geupdate is, als deze geupdate is (receiving 1 terug) dan gaan we de parameterlijst downloaden
*/


int get_type(uint8_t par_id)
{
	int found = 0;
	char buffer[JSON_MAX_SIZE];
	int type;
	int array_size;
	int succes;
	struct json_object *parameter_array;
	struct json_object *parameter;
	struct json_object *id;
	struct json_object *datatype;

	// File openen en naar buffer
	FILE *fptr = fopen("parameters.json" , "r");
	fread(buffer,JSON_MAX_SIZE,1,fptr);
	fclose(fptr);

	// De json gaat een array van structs zijn
	parsed_json = json_tokener_parse(buffer);
	succes = json_object_object_get_ex(parsed_json, "parameters" , &parameter_array);

	if(succes == 0)
	{
		printf("Failed to open json file with array tag \"parameters\". \n");
	}

	array_size = json_object_array_length(parameter_array);

	for(int i = 0; i < array_size; i++)
	{
		parameter = json_object_array_get_idx(parameter_array, i);
		json_object_object_get_ex(parameter, "id" , &id);
		int search_id;
		search_id = json_object_get_int(id);
		if(par_id == search_id)
		{
			json_object_object_get_ex(parameter, "datatype" , &datatype);
			i = array_size;
			type = json_object_get_int(datatype);
			found = 1;
		}
	}
	json_object_put(parsed_json);
	return type;
}
