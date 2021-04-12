#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "GSenseAPI.h"
#include "json_functions.h"
#include "config.h"


int main(int argc, char* argv[])
{
	// in cmd: ./set_parameter 1 0
	// argv[1] = 1 --> gaat parameter ID worden
	// argv[2] = 0 --> gaat parameter waarde worden

	if(argc < 2)
	{
		printf("Provide more ID's!\n");
		return 1;
	}
	int size = argc-1;
	argv = &argv[1];

	if(size > MAX_GET_BYTES){
		printf("you provided too musch parameters!\n");
		printf("MAX amount of parameters is %d, you provided %d parameters\n", MAX_GET_BYTES, size);
	}

	bytesToSend = malloc(MAX_GET_BYTES);
	bzero(bytesToSend, MAX_GET_BYTES);

	bytesToSend[0] = GET_ID;
	int id_count = 1;
	for(int i = 0; i < size; i ++){

		uint8_t parameterId = atoi(argv[i]);
		int type = get_type(parameterId);
		if(type == 0)
		{
			printf("PARAMETER is unknown with current ID: %d, please check if this ID is in the parameter list \n", parameterId);
		}

		else{
		}
		bytesToSend[id_count++] = parameterId;
	}

	get_parameter_command(bytesToSend, id_count, size);

	free(idArray);
	free(bytesToSend);
	return 0;
}
