#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>

// Memory struct
typedef struct
{
    uint8_t id;
    char descrip[5];
    int size;
} Memory;

//TCP
#define PORT 8080
#define MAX 2
#define SA struct sockaddr
#define STARTERKIT_IP "192.168.0.107"

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
int make_connection();
