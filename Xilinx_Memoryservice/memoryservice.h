#ifndef MEMORYSERVICE_H
#define MEMORYSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_udp.h>
#include <csp/csp_types.h>

#include "./src/csp_qfifo.h"
#include "controller.h"

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

union{
	uint8_t mem_list_bytes[sizeof(memory_list)];
	Memory mem_list[sizeof(memory_list)/sizeof(Memory)];
}memory_list_union;

//UDP
#define MY_ADDRESS 11
#define DEST_IP "0.0.0.0"
#define DEST_ADDR 6
#define DEST_PORT 10
#define TIMEOUT 1000

// MSG system again
#define DISCOVERY_ID 0
#define UPLOAD_ID 1
#define DOWNLOAD_ID 2

#define DDR_ID 0
#define FPGA_ID 1
#define FLASH_ID 2
#define RAM_ID 3

#define MAX_RECEIVING_BYTES 4096

// functies
void discovery(csp_iface_t * iface);
void upload(csp_iface_t * iface, uint8_t * data);
void download(csp_iface_t * iface, uint8_t * data);
void check_packet(csp_iface_t * iface, csp_packet_t * packet);

//UDP functions
csp_iface_t * init_udp(int length);
csp_packet_t * receiving_from_client();
void send_to_client(csp_iface_t * iface, uint8_t * data, int length);


#endif /* MEMORYSERVICE_H */
