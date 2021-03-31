#include "memoryservice.h"

int main(int argc, char *argv[])
{
	// Everything for TCP
	sockfd = setup_socket();
    uint8_t buff[5000];
	recv(sockfd, buff, sizeof(buff), 0);
	int case_id = buff[0];;

	switch(case_id)
	{
		case DISCOVERY_ID:
			printf("ID recv for discovery \n");
			discovery();
			break;
		case UPLOAD_ID:
			printf("ID recv for uploading \n");
			upload(buff);
			break;
		case DOWNLOAD_ID:
			printf("ID recv for downloading \n");
			download(buff);
			break;
		default:
			printf("Default case \n");
			break;
	}
}

void discovery()
{
	send(sockfd, memory_list, sizeof(memory_list), 0);
}

void upload(uint8_t buff[])
{
	uint8_t mem_id;
	uint32_t offset;
	uint32_t size;

	mem_id = buff[1];
	offset = buff[2] | (buff[3] << 8) | (buff[4] << 16) | (buff[5] << 24);
	size = buff[6] | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24);

	uint8_t data_buffer[size];
	for(int i = 0; i < size; i++)
	{
		data_buffer[i] = buff[10 + i];
	}

	switch(mem_id)
	{
		case DDR_ID:
			printf("Uploading to DDR \n");
			upload_ddr(offset, size,data_buffer);
			break;
		case FPGA_ID:
			printf("Uploading to FPGA \n");
			upload_fpga(offset, size,data_buffer);
			break;
		case FLASH_ID:
			printf("Uploading to FLASH \n");
			break;
		case RAM_ID:
			printf("Uploading to RAM \n");
			upload_ram(offset, size,data_buffer);
			break;
		default:
			printf("Default case \n");
			break;
	}

}

void download(uint8_t buff[])
{
	uint8_t mem_id;
	uint32_t offset;
	uint32_t size;

	uint8_t *send_datastream;

	mem_id = buff[1];
	offset = buff[2] | (buff[3] << 8) | (buff[4] << 16) | (buff[5] << 24);
	size = buff[6] | (buff[7] << 8) | (buff[8] << 16) | (buff[9] << 24);

	switch(mem_id)
	{
		case DDR_ID:
			printf("Downloading from DDR from offset %d with size %d bytes.\n", offset,size);
			send_datastream = download_stream_ddr(offset, size);
			break;
		case FPGA_ID:
			printf("Downloading from FPGA \n");
			send_datastream = download_stream_fpga(offset, size);
			break;
		case FLASH_ID:
			printf("Downloading from FLASH \n");
			break;
		case RAM_ID:
			printf("Downloading from RAM \n");
			send_datastream = download_stream_ram(offset, size);
			break;
		default:
			printf("Default case \n");
			break;
	}

	uint8_t send_buffer[size];

	// You can check the send buffer
	for(uint32_t i = 0; i < size; i++)
	{
		send_buffer[i] = *(send_datastream + i);
		// printf("The %d 'th element is : %c \n", i, send_buffer[i]);
	}

	send(sockfd, send_buffer, sizeof(send_buffer), 0);

}
