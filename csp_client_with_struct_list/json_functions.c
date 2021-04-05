#include "json_functions.h"
#include "GSenseAPI.h"
#include "config.h"

/*
Deze functie krijgt de byte array van de server binnen als antwoord op print_parameter en gebruikt de union parameter_list_union om de byte array snel om te zetten in de
parameter_t struct list.
Daarna roept hij de functie get_par_list_in_json aan om deze struct list weg te schrijven in een json file.
*/
void store_list_from_bytes(uint8_t * data, int length)
{

	struct json_object * jobj;
	struct json_object * jobjarray;

	jobj = json_object_new_object();
	jobjarray = json_object_new_array();

	printf("length: %d\n", length);

	for(int i = 1; i < length; i++){
		parameter_list_union.par_list_bytes[i-1] = data[i];
		printf("data[%d]: %d\n", i, data[i]);
	}

	printf("parameterlist[0].description: %s\n", parameter_list_union.par_list[0].description);


	get_par_list_in_json(jobjarray, parameter_list_union.par_list, length/sizeof(parameter_t));

	json_object_object_add(jobj, "parameters", jobjarray);

	json_object_to_file_ext("parameters.json", jobj, JSON_FLAG);

	json_object_put(jobj);
}

/*
Wanneer de byte array komende van de server als antwoord op print_parameter omgezet is in de struct list parameter_t []
is het noodzakelijk om deze struct list weg te schrijven in de json file.
Deze functie gaat alle keys af horende bij elke variable in de struct en schrijft de waarde daarin weg.
*/
//SYNCHRONIZATION NEEDED
void get_par_list_in_json(json_object * jobjarray, parameter_t * parameterlist, int size){



	for(int i = 0; i < size; i++){
		struct json_object * jparameter = json_object_new_object();

		//putting the values from the struct list created in the for loop in store_list_from_bytes in the json file so we wont need the struct further on anymore
		//If a new parameter is added on the server side, this needs to be added here as well
		json_object_object_add(jparameter, "id", json_object_new_int(parameterlist[i].id));
		json_object_object_add(jparameter, "description", json_object_new_string(parameterlist[i].description));
		json_object_object_add(jparameter, "datatype", json_object_new_int(parameterlist[i].datatype));
		json_object_object_add(jparameter, "offset", json_object_new_int(parameterlist[i].offset));
		json_object_object_add(jparameter, "value", json_object_new_int(parameterlist[i].value));
		json_object_object_add(jparameter, "updated", json_object_new_boolean(parameterlist[i].updated));

		//adding the full jparameter to jobjarray
		json_object_array_add(jobjarray, jparameter);
	}
}

/*
De server heeft de values horende bij de ID's teruggestuurd als antwoord op de get_parameter <ID array> commando.
Hij gaat naar de eerste byte kijken die de eerste ID voorstelt, haalt dan de juiste json object geassocieerd hiermee uit zodat hij
met de functie set_value_in_jobject het datatype kan laten kloppen.
*/
void add_values(uint8_t * data, int length){

	struct json_object * jobj;
	jobj = json_object_new_object();
	json_object * jobj_array = get_json_from_file();
	int json_array_size = json_object_array_length(jobj_array);

	// i = 1 omdat i = 0 de msg_id is
	for(int i = 1; i < length;){
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

/*
Deze functie verandert de json key "value" in de json file op basis van zijn datatype.
Sommige json objects hebben value die bestaat uit 1 byte en sommige uit meerdere bytes.
Deze functie wordt opgeroepen nadat de hogere functie de ID eruit gehaald heeft. de index_of_data staat dus op de eerste value byte.
Het is aan deze functie om deze juiste aantal bytes volgende op de index_of_data index eruit te halen.
*/
int set_value_in_jobject(json_object * jparameter, uint8_t * data, int index_of_data){

	json_object * jdatatype = json_object_object_get(jparameter, "datatype");
	json_object * jvalue = json_object_object_get(jparameter, "value");
	int datatype = json_object_get_int(jdatatype);
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

/*
Deze functie wordt opgeroepen nadat de server de waardes heeft aangepast op de camera zelf
Als de waardes aangepast zijn wordt de "updated" key op 1 gezet om te laten weten dat deze aangepast zijn.
*/
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

/*
Deze functie print de json object als een string met de json_object_to_json_string_ext command
*/
void print_list()
{

	struct json_object * jobj;
	jobj = json_object_new_object();
	struct json_object * jobj_array = get_json_from_file(); //getting the amount of different id's
	json_object_object_add(jobj, "parameters", jobj_array);
	printf("%s", json_object_to_json_string_ext(jobj, JSON_FLAG));
	json_object_put(jobj);
}

/*
Deze functie gaat de json object genaamd "parameters" uit het json object halen die in de .json file zit. het json object "parameters" is ook de enige die hierin zit.
De reden dan alleen "parameters" hierin zit is omdat dit de titel is van het json object.
Alles onder "parameters" is een json array.
*/
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

/*
Deze functie gaat het datatype van de parameter returnen op basis van zijn id
Dit wordt tot nu toe alleen gebruikt bij de commands
Bij get_parameter is dit om te kijken of de id die je hebt ingevuld wel echt bestaat
bij set_parameter is dit om te kijken hoeveel bytes we moeten voorzien aan de value die we hebben ingegeven
*/
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
