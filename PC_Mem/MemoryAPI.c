#include "MemoryAPI.h"


void discovery_memory()
{

	uint8_t send_buffer[1];

	send_buffer[0] = DISCOVERY_ID;

	// Ask for memory list
	csp_iface_t * iface = init_udp(DIFFERENT_MEMORYS*sizeof(Memory));
	send_to_server(iface, send_buffer, sizeof(send_buffer));

	// UDP Receive memory_list
	csp_packet_t * packet = received_from_server();
	for(int i = 0; i < packet->length; i++){
		memory_list_union.mem_list_bytes[i] = packet->data[i];
	}

  // Print de memory list
	print_memory(memory_list_union.mem_list);

}

void print_memory(Memory memory_list[])
{
	printf(" ------------- MEMORY DISCOVERY --------------- \n");
	printf(" ID \t | Memory \t | Size (Mb) \n");

	for(int i = 0; i < DIFFERENT_MEMORYS ;i++)
	{
		uint8_t id = memory_list[i].id;
		int size = memory_list[i].size;
		printf(" %d \t | %s \t | %d \n", id, memory_list[i].descrip,size);
	}
}

void upload_memory(uint8_t mem_id, uint32_t offset, char *file_name)
{
    uint8_t * data_stream;
    uint32_t data_size;
	uint8_t *preamble;

    data_stream = file_to_byte(file_name);
    data_size = get_file_size(file_name);

    // Prepare preamble of send_buffer
    uint8_t send_buffer[data_size + 10];
    preamble = prepare_preamble(UPLOAD_ID,mem_id,offset,data_size);

	for(int i = 0; i < 10; i++)
	{
		send_buffer[i] = *(preamble + i);
	}

    // Add file to the preamble
    for(int i = 0; i < data_size; i++)
    {
        send_buffer[i+10] = *(data_stream + i);
        printf("send_buffer index 10 + %d : %c \n", i, *(data_stream + i));
    }

		csp_iface_t * iface = init_udp(sizeof(send_buffer));
    send_to_server(iface, send_buffer, sizeof(send_buffer));
}

uint8_t * prepare_preamble(uint8_t msg_id, uint8_t mem_id, uint32_t offset, uint32_t data_size)
{
	uint8_t *offset_array;
	uint8_t *size_array;

	uint8_t *preamble = (uint8_t *) malloc(sizeof(uint8_t)*10);

	preamble[0] = msg_id;
	preamble[1] = mem_id;

	// offset naar 4 bytes
	offset_array = (uint8_t*)(&offset);
	for(int i = 0; i < 4; i++)
	{
		preamble[i+2] = offset_array[i];
	}

	// size naar 4 bytes
	size_array = (uint8_t*)(&data_size);
	for(int i = 0; i < 4; i++)
	{
		preamble[i+6] = size_array[i];
	}

	return preamble;
}

void download_memory(uint8_t mem_id, uint32_t offset, uint32_t data_size, char *file_name)
{
	uint8_t *data_stream;
	uint8_t *offset_array;
	uint8_t *size_array;

	// 10 bytes extra for info
	uint8_t send_buffer[data_size + 10];
	send_buffer[0] = DOWNLOAD_ID;
	send_buffer[1] = mem_id;

	// offset naar 4 bytes
	offset_array = (uint8_t*)(&offset);
	for(int i = 0; i < 4; i++)
	{
		send_buffer[i+2] = offset_array[i];
	}

	// size naar 4 bytes
	size_array = (uint8_t*)(&data_size);
	for(int i = 0; i < 4; i++)
	{
		send_buffer[i+6] = size_array[i];
	}

	csp_iface_t * iface = init_udp(sizeof(send_buffer));
	send_to_server(iface, send_buffer, sizeof(send_buffer));

	// Wachten op het antwoord
	csp_packet_t * recv_packet = received_from_server();
	printf("First datachunk is : %c \n", recv_packet->data[0]);

	// Antwoord storen in file
	FILE *fp;
	fp = fopen(file_name, "w");
	fwrite(recv_packet->data, 1, recv_packet->length ,fp);
	fclose(fp);

}


// Om de command line argument bij upload functie naar string om te zetten
char * get_string_cmd_upload(int argc, char* argv[])
{
	char *cmdstring;
    int strsize = 0;
    for(int i = 3; i < argc; i++)
    {
        strsize += strlen(argv[i]);
        if (argc > i + 1) { strsize++; }
    }

    cmdstring = malloc(strsize);
    cmdstring[0] = '\0';

    for(int i = 3; i < argc; i++)
    {
        strcat(cmdstring,argv[i]);
        if (argc > i+1) { strcat(cmdstring, " ");}
    }

    return cmdstring;
}


//. Om de command line argument bij download functie naar string om te zetten
char * get_string_cmd_download(int argc, char* argv[])
{
	char *cmdstring;
    int strsize = 0;
    for(int i = 4; i < argc; i++)
    {
        strsize += strlen(argv[i]);
        if (argc > i + 1) { strsize++; }
    }

    cmdstring = malloc(strsize);
    cmdstring[0] = '\0';

    for(int i = 4; i < argc; i++)
    {
        strcat(cmdstring,argv[i]);
        if (argc > i+1) { strcat(cmdstring, " ");}
    }

    return cmdstring;
}

// Zet eender welke file om naar een byte array
uint8_t * file_to_byte(char *file_name)
{
    uint8_t *buffer;
    FILE *fileptr;
    long filelen;


    fileptr = fopen(file_name,"r");
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (uint8_t *)malloc(filelen * sizeof(uint8_t)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr);

    fclose(fileptr);

    return buffer;

}

uint32_t get_file_size(char *file_name)
{
    uint32_t file_size;
    FILE *fileptr;
    fileptr = fopen(file_name,"r");
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    file_size = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file
    fclose(fileptr);
    return file_size;
}


csp_iface_t * init_udp(int length){

	if(csp_buffer_init(1, length)<0)
		printf("could not initialize buffer\n");

	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
	if(csp_init(&csp_conf) < 0)
	printf("could not initialize csp\n");

	static csp_iface_t udp_iface;
	csp_if_udp_init(&udp_iface, DEST_IP );
	csp_rtable_set(0,0, &udp_iface, CSP_NODE_MAC);

	return &udp_iface;

}


void send_to_server(csp_iface_t * iface, uint8_t * data, int length){
	csp_buffer_init(1,length);
	csp_packet_t * packet = csp_buffer_get(length);
	for(int i = 0; i < length; i++){
		packet->data[i] = data[i];
	}
	packet->length = length;
	csp_if_udp_tx(iface, packet, TIMEOUT);
}


csp_packet_t * received_from_server(){

		csp_packet_t * recv_packet;

		while(1){
			csp_qfifo_t input;
			if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
				continue;
			}
			printf("aaa\n");
			recv_packet = input.packet;
			break;
		}
		return recv_packet;

}
