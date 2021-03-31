#include "MemoryAPI.h"


void discovery_memory()
{
	int sockfd, size;
	uint8_t id;
	uint8_t send_buffer[1];
	Memory memory_list[DIFFERENT_MEMORYS];

	send_buffer[0] = DISCOVERY_ID;

	// TCP Send message
	sockfd = make_connection();
	send(sockfd,send_buffer, sizeof(send_buffer),0);

	// TCP Recieve memory_list
	recv(sockfd, memory_list, sizeof(memory_list), 0);

    // Print de memory lijst
	print_memory(memory_list);

	// TCP socket sluiten
	close(sockfd);
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


    // Sending the buffer over TCP
    int sockfd = make_connection();
    send(sockfd,send_buffer, sizeof(send_buffer),0);
    close(sockfd);
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
	// printf("Filelength bedraagt %d bytes \n", data_size);

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

	// Sending the buffer over TCP
	int sockfd = make_connection();
	send(sockfd,send_buffer, sizeof(send_buffer),0);

	// Wachten op het antwoord
	uint8_t recv_buffer[data_size];
	recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);
	printf("First datachunk is : %c \n", recv_buffer[0]);

	// Antwoord storen in file
	FILE *fp;
	fp = fopen(file_name, "w");
	fwrite(recv_buffer, sizeof(recv_buffer[0]), sizeof(recv_buffer)/sizeof(recv_buffer[0]),fp);

	// Socket sluiten
	close(sockfd);

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


// TCP_USE
int make_connection()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

	// zien dat we steeds met een cleane socket beginnen
	int true = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("TCP : socket creation failed...\n");
        exit(0);
    }
    else
        printf("TCP : Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // HIER MOETEN WE IP VAN DE PC GAAN INGEVEN DENK

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(STARTERKIT_IP);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("TCP : connection with the server failed...\n");
        int error_code = connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
        printf("Failed with error code : %d \n", error_code);
        exit(0);
    }
    else
        printf("TCP : connected to the server..\n");

    return sockfd;
}
