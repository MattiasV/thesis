#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GSenseAPI.h"
#include "json_functions.h"
#include "config.h"

int main(int argc, char* argv[])
{
	printf("\n\n");
	// in cmd: ./set_parameter 1 0
	// argv[1] = 1 --> gaat parameter ID worden
	// argv[2] = 0 --> gaat parameter waarde worden

	if(argc < 3){
		printf("Provide an ID and a VALUE!\n");
		return 1;
	}

	if(argc%2 == 0)
	{
		printf("  Wrong input! You didnt provide the input in following format: \n");
		printf("  ./set_parameter <id> <value> <id> <value> ...\n");
		return 1;
	}

	int size = argc-1;
	int length = 0;

	bytesToSend = malloc(MAX_SET_BYTES);
	idArray = malloc(size/2);

	bytesToSend[0] = SET_ID;
	int byteindex = 1; //start at index 1 because SET_ID is on 0
	for(int i = 1; i < size; i += 2){

		uint8_t id = atoi(argv[i]);
		int type = get_type(id);

		if(type == 0)
		{
			printf("  PARAMETER is unknown, try again \n");
		}

		if(type <= 2)
		{
			double value = atoi(argv[i+1]);

			//ERROR_CHECK
			if(value > 255 || value < 0 && type == 1)
				printf("  Value at position %d is not in the range of type uint8_t!\n", i+1);
			else if(value < -128 || value > 128 && type == 2)
				printf("  Value at position %d is not in the range of type int8_t!\n", i+1);

			else{
				idArray[i/2] = id;
				bytesToSend[byteindex++] = id;
				bytesToSend[byteindex++] = value;
			}

		}

		else if(type <= 4)
		{
			int value = atoi(argv[i+1]);
			//ERROR_CHECK
			if(value > 65535 || value < 0 && type == 3)
				printf("  Value at position %d is not in the range of type uint16_t!\n", i+1);
			else if(value < -32768 || value > 32767 && type == 4)
				printf("  Value at position %d is not in the range of type int16_t!\n", i+1);

			else{
				idArray[i/2] = id;
				bytesToSend[byteindex++] = id;
				if(value >= 0)
					fourBytesUnion.u16bytes[0] = value;
				else
					fourBytesUnion.i16bytes[0] = value;
				for(int j = 0; j < 2; j++){
					bytesToSend[byteindex++] = fourBytesUnion.u8bytes[j];
				}
			}
		}

		else if(type <= 6)
		{
			double value = atof(argv[i+1]);

			//ERROR_CHECK
			if(value > 4294967295 || value < 0 && type == 5) //onnodige check omdat value een int is...
				printf("  Value at position %d is not in the range of type uint32_t!\n", i+1);
			else if(value < -2147483648 || value > 2147483647 && type == 6)
				printf("  Value at position %d is not in the range of type int32_t!\n", i+1);

			else{
				idArray[i/2] = id;
				bytesToSend[byteindex++] = id;
				if(value >= 0)
					fourBytesUnion.u32bytes = value;
				else
					fourBytesUnion.i32bytes = value;
				for(int j = 0; j < 4; j++){
					bytesToSend[byteindex++] = fourBytesUnion.u8bytes[j];
				}
			}
		}

		if(type == 7)
		{
			float value = atof(argv[i+1]);
			idArray[i/2] = id;
			bytesToSend[byteindex++] = id;
			fourBytesUnion.fbytes = value;
			for(int j = 0; j < 4; j++){
				bytesToSend[byteindex++] = fourBytesUnion.u8bytes[j];
			}
		}
	}

	start_communication(bytesToSend, byteindex, size/2);
	free(bytesToSend);
	return 0;
}
