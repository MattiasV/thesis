#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GSenseAPI.h"
#include "jsonFunctions.h"

int main(int argc, char* argv[])
{
	// in cmd: ./set_parameter 1 0
	// argv[1] = 1 --> gaat parameter ID worden
	// argv[2] = 0 --> gaat parameter waarde worden

	if(argc < 3){
		printf("Provide an ID and a VALUE!\n");
		return 1;
	}

	if(argc%2 == 0)
	{
		printf("Wrong input! You didnt provide the input in following format: \n");
		printf("./set_parameter <id> <value> <id> <value> ...\n");
		return 1;
	}

	argv = &argv[1];
	int size = argc-1;
	int length = 0;

	bytesToSend = malloc(MAX_SET_BYTES);
	idArray = malloc(size/2);

	bytesToSend[0] = SET_ID;

	for(int i = 0; i < size; i += 2){

		uint8_t parameterId = atoi(argv[i]);
		const char* type = get_type(parameterId);
		//printf("parameterId: %d, type: %s\n",parameterId, type);

		if(strcmp("UNKNOWN",type) == 0)
		{
			printf("PARAMETER is unknown, try again \n");
		}

		if(strcmp("uint8_t",type) == 0)
		{
			int parameterCheck = atoi(argv[i+1]);
			//printf("parameterCheck: %d\n", parameterCheck);
			if(parameterCheck <= 255 && parameterCheck >= 0)
			{
				uint8_t argvSending[2];
				bzero(&argvSending, sizeof(argvSending));
				uint8_t id, value;
				sscanf(argv[i], "%hhd", &id);
				sscanf(argv[i+1], "%hhd", &value);
				idArray[i/2] = id;
				argvSending[0] = id;
				argvSending[1] = value;
				memcpy(bytesToSend+i+1, argvSending, sizeof(argvSending));
				length += 2;
			}
			else
			{
				printf("Not a valid uint8 value with current id: %d\n", parameterId);
			}
		}

		if(strcmp("uint32_t",type) == 0)
		{
			int parameterCheck = atoi(argv[1]);
			if(parameterCheck >= 0)
			{
				uint32_t argvSending[2];
				uint8_t id;
				uint32_t value;
				sscanf(argv[i], "%hhd", &id);
				sscanf(argv[i+1], "%d", &value);
				idArray[i/2] = id;
				argvSending[0] = id;
				argvSending[1] = value;
				memcpy(bytesToSend+i+1, argvSending, sizeof(argvSending));
				length += 5;
			}
			else
			{
				printf("Not a valid uint32 value with current id: %d\n", parameterId);
			}
		}

		if(strcmp("float",type) == 0)
		{

			float argvSending[2];
			uint8_t id;
			float value;
			sscanf(argv[i], "%hhd", &id);
			sscanf(argv[i+1], "%f", &value);
			idArray[i/2] = id;
			argvSending[0] = id;
			argvSending[1] = value;
			memcpy(bytesToSend+i+1, argvSending, sizeof(argvSending));
			length += 5;
		}
	}
	// for(int i = 0; i < size; i++){
	// 	printf("aaaaaaaaaa: %d\n", bytesToSend[i]);
	// }
	sendToServer(bytesToSend, length, size/2);
	free(bytesToSend);
	return 0;
}
