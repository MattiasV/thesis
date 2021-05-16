#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "GSenseAPI.h"
#include "json_functions.h"

int main(int argc, char* argv[])
{
	if(argc >= 2)
	{
		int refresh = load_list();
	}
	bytesToSend = malloc(MAX_PRINT_BYTES);
	bzero(bytesToSend, MAX_PRINT_BYTES);
	bytesToSend[0] = DOWNLOAD_ID;
	start_communication(bytesToSend, 1, 0);
	printf("\n\n");
	print_list();

	free(bytesToSend);
	return 0;
}
