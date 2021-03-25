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
	int byteindex = 1; //start at index 1 because SET_ID is on 0
	for(int i = 1; i < size; i += 2){

		uint8_t id = atoi(argv[i]);
		int type = get_type(id);
		//printf("parameterId: %d, type: %s\n",parameterId, type);

		if(type == 0)
		{
			printf("PARAMETER is unknown, try again \n");
		}

		if(type <= 2)
		{
			int value = atoi(argv[i+1]);
			//printf("parameterCheck: %d\n", parameterCheck);

			idArray[i/2] = id;
			bytesToSend[byteindex++] = id;
			length++;
			bytesToSend[byteindex++] = value
			length++;
		}

		else if(type > 2 && type <= 4)
		{
			int value = atoi(argv[i]);
			idArray[i/2] = id;
			bytesToSend[byteindex++] = id;
			length++;
			if(value >= 0)
				fourBytesUnion.u16bytes[0] = value;
			else
				fourBytesUnion.i16bytes[0] = value;
			for(int j = 0; j < 2; j++){
				bytesToSend[byteindex++] = fourBytesUnion.u8bytes[j];
				length++
			}
		}

		else if(type > 4 && type <= 6)
		{
			int value = atoi(argv[i]);
			idArray[i/2] = id;
			bytesToSend[byteindex++] = id;
			length++;
			if(value >= 0)
				fourBytesUnion.u32bytes[0] = value;
			else
				fourBytesUnion.i32bytes[0] = value;
			for(int j = 0; j < 4; j++){
				bytesToSend[byteindex++] = fourBytesUnion.u8bytes[j];
				length++;
			}
		}

		if(type == 7)
		{
			float value = atoi(argv[i]);
			idArray[i/2] = id;
			bytesToSend[byteindex++] = id;
			length++;
			fourBytesUnion.fbytes = value;
			for(int j = 0; j < 4; j++){
				bytesToSend[index++] = fourBytesUnion.u8bytes[j];
				length++;
			}
		}
	}

	sendToServer(bytesToSend, length, size/2);
	free(bytesToSend);
	return 0;
}
