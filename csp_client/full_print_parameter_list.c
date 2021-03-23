#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "GSenseAPI.h"
#include "jsonFunctions.h"

int main(int argc, char* argv[])
{
	if(argc >= 2)
	{
		int refresh = load_list();
	}

	download_list();

	print_list();

	return 0;
}
