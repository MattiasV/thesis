#include "jsonFunctions.h"
#include "GSenseAPI.h"

void store_list(uint8_t * data)
{
	// JSON file gaan openen
	int num;
	FILE *fptr;

	fptr = fopen("parameters.json" , "w+");
	fprintf(fptr,"{ \n");
	fprintf(fptr,"\"parameters\": [{ \n");

	// De parameters gaan invullen
	while(data != NULL)
	{
		int id,  size, value;
		bool updated;
		char description[MAX_DESCRIPTION_SIZE], datatype[MAX_TYPE_SIZE];

		data = getDataString(&id, description, datatype, &size, &value, &updated, data);
		//printf("id: %d, d_type: %s, description: %s, size: %d, value: %d, updated: %d\n", id, datatype, description, size, value, updated);

		// ALLe info in JSON zetten, bij de laatste geen komma op het einde!!
		fprintf(fptr, "\t \t \"id\":\"%d\",\n", id);
		fprintf(fptr, "\t \t \"description\":\"%s\",\n", description);
		fprintf(fptr, "\t \t \"type\":\"%s\",\n", datatype);
		fprintf(fptr, "\t \t \"size\":\"%d\",\n", size);
		fprintf(fptr, "\t \t \"value\":\"%d\",\n", value);
		fprintf(fptr, "\t \t \"updated\":\"%d\"\n", updated);

		if(data != NULL)
			fprintf(fptr, "\t }, { \n");
	}

	fprintf(fptr,"}] \n");
	fprintf(fptr, "}\n");

	// De file sluiten
	fclose(fptr);

}

uint8_t * getDataString(int * id, char description[MAX_DESCRIPTION_SIZE], char datatype[MAX_TYPE_SIZE], int * size, int * value, bool * updated, uint8_t * data){

	int caseID = 0;
	int done = 0;
	int index = 0;
	char idchar[sizeof(int)], temp_type[MAX_TYPE_SIZE], temp_desc[MAX_DESCRIPTION_SIZE], sizechar[sizeof(int)], valuechar[sizeof(int)], charupdated;
	uint8_t * dataptr;

	dataptr = data;

	for(int i = 0; data[i] != '\0' && done == 0; i++){

		switch(caseID){
			case 0:
				if(data[i] == ','){
					caseID ++;
					sscanf(idchar, "%d", id);
					memset(&idchar[0], 0, sizeof(idchar));
					index = 0;
					break;
				}
				idchar[index++] = data[i];
				break;

			case 1:
				if(data[i] == ','){
					caseID ++;
					temp_desc[index] = '\0'; //To make sure the char array ends here and not with some strange characters
					strcpy(description, temp_desc);
					memset(&temp_desc[0], 0, sizeof(temp_desc));
					index = 0;
					break;
				}
				temp_desc[index++] = data[i];
				break;

			case 2:
				if(data[i] == ','){
					caseID ++;
					temp_type[index] = '\0'; //To make sure the char array ends here and not with some strange characters
					strcpy(datatype, temp_type);
					memset(&temp_type[0], 0, sizeof(temp_type));
					index = 0;
					break;
				}
				temp_type[index++] = data[i];
				break;

			case 3:
				if(data[i] == ','){
					caseID ++;
					sscanf(sizechar, "%d", size);
					memset(&sizechar[0], 0, sizeof(sizechar));
					index = 0;
					break;
				}
				sizechar[index++] = data[i];
				break;

			case 4:
				if(data[i] == ','){
					caseID ++;
					sscanf(valuechar, "%d", value);
					memset(&valuechar[0], 0, sizeof(valuechar));
					index = 0;
					break;
				}
				valuechar[index++] = data[i];
				break;

			case 5:
				if(data[i] == ','){
					caseID ++;
					*updated = charupdated != '0';
					memset(&charupdated, 0, sizeof(charupdated));
					break;
				}
				charupdated = data[i];
				break;

			case 6:
				if(data[i] == '\0'){
					done = 0;
					break;
				}
				dataptr = &data[i];
				done = 1;
				break;

			default:
				printf("Default\n");
		}
	}

	if(done == 0)
		return NULL;
	return dataptr;

}

void addValues(uint8_t * data, int length){

	struct json_object * json_object_list[JSON_OBJ_LIST_SIZE];
	char * serialized_list = malloc(JSON_MAX_SIZE); // this is the list where all the variables will be stored as a string.
	uint8_t * serialized_list_bytes = malloc(JSON_MAX_SIZE); //this will be the serialized list put in a byte array to be send to store_list()
	char * temp_serialized_list = malloc(JSON_OBJ_LIST_SIZE);
	bzero(serialized_list, JSON_MAX_SIZE);
  bzero(serialized_list_bytes, JSON_MAX_SIZE);

	int size = getValuesFromJson(json_object_list);

	for(int i = 0; i < size; i++){

		bzero(temp_serialized_list, JSON_OBJ_LIST_SIZE);
		int par_id = json_object_get_int(json_object_list[i*(size + 1) + ID_INDEX]);
		const char * description = json_object_get_string(json_object_list[i*(size + 1) + DESC_INDEX]);
		const char * type = json_object_get_string(json_object_list[i*(size + 1) + TYPE_INDEX]);
		int data_size = json_object_get_int(json_object_list[i*(size + 1) + SIZE_INDEX]);
		int value = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		bool updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);

		int j = 0;
		while(j < length){
			//Check of the data[j] (which represents always the id, this is made sure with the j+=...) is the same as the current id
			if(data[j] == par_id && updated != true){//Nieuwe value wordt niet geupdatet wanneer er eigenlijk een nieuwe value nog moet geschreven worden aan de server.

				if(strcmp("uint8_t",type) == 0){
					value = data[j+1];
					j += 2*sizeof(uint8_t); // 1 byte for id and 1 byte for value

				}else{
					for(int k = 0; k < 4; k++){
						fourBytesUnion.u8bytes[k] = data[j+1+k]; //fill the uint8_t [4] array with the next 4 bytes to get the uint32_t or the float
					}

					if(strcmp("uint32_t",type) == 0){
						value = fourBytesUnion.u32bytes;
					}else{
						value = fourBytesUnion.fbytes;
					}
					j += (sizeof(uint8_t) + sizeof(uint32_t));
				}
			}
		}


		sprintf(temp_serialized_list,"%d,%s,%s,%d,%d,%d,", par_id, description, type, data_size, value, updated); // puts variables in string

		if(size == 0)
			strcpy(serialized_list, temp_serialized_list);
		else
			strcat(serialized_list, temp_serialized_list);
	}

	snprintf(serialized_list_bytes, JSON_MAX_SIZE, "%s", serialized_list);

	store_list(serialized_list_bytes);

	free(temp_serialized_list);
	free(serialized_list);
	free(serialized_list_bytes);


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
		const char * type = json_object_get_string(json_object_list[i*(size + 1) + TYPE_INDEX]);
		int data_size = json_object_get_int(json_object_list[i*(size + 1) + SIZE_INDEX]);
		int value = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		bool updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);

		for(int j = 0; j < amountOfIds; j++){
			if(idArray[i] == par_id)
				updated = true;
		}
		sprintf(temp_serialized_list,"%d,%s,%s,%d,%d,%d,", par_id, description, type, data_size, value, updated); // puts variables in string

		if(size == 0)
			strcpy(serialized_list, temp_serialized_list);
		else
			strcat(serialized_list, temp_serialized_list);

	}

	snprintf(serialized_list_bytes, JSON_MAX_SIZE, "%s", serialized_list);

	store_list(serialized_list_bytes);

	json_object_put(parsed_json);

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
		const char * type = json_object_get_string(json_object_list[i*(size + 1) + TYPE_INDEX]);
		int data_size = json_object_get_int(json_object_list[i*(size + 1) + SIZE_INDEX]);
		int value = json_object_get_int(json_object_list[i*(size + 1) + VALUE_INDEX]);
		int updated = json_object_get_int(json_object_list[i*(size + 1) + UPDATED_INDEX]);


		printf(" %d \t | %s \t | %s \t | %d \t\t | %d \t \n",par_id,description,type,data_size, value); //printing the list

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
	struct json_object *size;
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
		json_object_object_get_ex(parameter, "size" , &size);
		json_object_object_get_ex(parameter, "value" , &value);
		json_object_object_get_ex(parameter, "updated" , &updated);


		json_object_list[i*(array_size + 1) + ID_INDEX] = id;
		json_object_list[i*(array_size + 1) + DESC_INDEX] = description;
		json_object_list[i*(array_size + 1) + TYPE_INDEX] = type;
		json_object_list[i*(array_size + 1) + SIZE_INDEX] = size;
		json_object_list[i*(array_size + 1) + VALUE_INDEX] = value;
		json_object_list[i*(array_size + 1) + UPDATED_INDEX] = updated;
	}

	return array_size;

}

/**
Hier gaan we een datapakket [0] sturen, die simpelweg aan starterkit vraagt of de parameterlijst
geupdate is, als deze geupdate is (receiving 1 terug) dan gaan we de parameterlijst downloaden
*/


const char* get_type(uint8_t par_id)
{
	int found = 0;
	char buffer[JSON_MAX_SIZE];
	char * typechar;
	int array_size;
	int succes;
	struct json_object *parameter_array;
	struct json_object *parameter;
	struct json_object *id;
	struct json_object *type;

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
			json_object_object_get_ex(parameter, "type" , &type);
			i = array_size;
			typechar = (char * )json_object_get_string(type);
			found = 1;
		}
	}
	json_object_put(parsed_json);
	return typechar;
}
