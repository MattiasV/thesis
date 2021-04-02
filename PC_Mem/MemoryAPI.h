#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_udp.h>
#include <csp/csp_types.h>

#include "./src/csp_qfifo.h"

// Memory struct
typedef struct
{
    uint8_t id;
    char descrip[5];
    int size;
} Memory;

union{
	uint8_t mem_list_bytes[4*sizeof(Memory)];
	Memory mem_list[4];
}memory_list_union;


//UDP
#define MY_ADDRESS 11
#define DEST_IP "192.168.10.103"
#define DEST_ADDR 6
#define DEST_PORT 10
#define TIMEOUT 1000

// Communicatiesysteem
#define DISCOVERY_ID 0
#define UPLOAD_ID 1
#define DOWNLOAD_ID 2

// Memory constants
#define DIFFERENT_MEMORYS 4

// Discovery service
void discovery_memory();
void print_memory(Memory memory_list[]);

// Upload function
void upload_memory(uint8_t mem_id, uint32_t offset, char *file_name);
uint8_t * prepare_preamble(uint8_t msg_id, uint8_t mem_id, uint32_t offset, uint32_t data_size);
char * get_string_cmd_upload(int argc, char* argv[]);
uint8_t * file_to_byte(char *file_name);
uint32_t get_file_size(char *file_name);

// Download functions
void download_memory(uint8_t mem_id, uint32_t offset, uint32_t data_size, char *file_name);
char * get_string_cmd_download(int argc, char* argv[]);

// Communication function
csp_iface_t * init_udp();
void send_to_server(csp_iface_t * iface, uint8_t * data, int length);
csp_packet_t * received_from_server();
