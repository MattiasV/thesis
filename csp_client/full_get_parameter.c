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

	for(int i = 0; i < size; i ++){

		uint8_t parameterId = atoi(argv[i]);
		int type = get_type(parameterId);
		if(type == 0)
		{
			printf("PARAMETER is unknown with current id: %d, try again sending this id again \n", parameterId);
		}

		else{
		}
		bytesToSend[i+1] = parameterId;
	}
	send_to_server(bytesToSend, argc, size);

	free(idArray);
	free(bytesToSend);
	return 0;
}
