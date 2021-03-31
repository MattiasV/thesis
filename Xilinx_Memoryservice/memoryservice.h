#ifndef MEMORYSERVICE_H
#define MEMORYSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "controller.h"
#include "server.h"


typedef struct
{
	uint8_t id;
	char descrip[5];
	int size;
} Memory;

Memory memory_list[] = {
		{0,"DDR ",128},
		{1,"FPGA",400},
		{2,"Flash",50},
		{3,"RAM ",32}
};

// TCP
int sockfd;


// MSG system again
#define DISCOVERY_ID 0
#define UPLOAD_ID 1
#define DOWNLOAD_ID 2

#define DDR_ID 0
#define FPGA_ID 1
#define FLASH_ID 2
#define RAM_ID 3


// functies
void discovery();
void upload(uint8_t buffer[]);
void download(uint8_t buff[]);

#endif /* MEMORYSERVICE_H */
