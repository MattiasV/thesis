#include "memoryservice.h"

int main(int argc, char *argv[])
{


	// SETTING UP UDP CSP SERVER AND LISTEN FOR INCOMMING MSG

	if(csp_buffer_init(1, MAX_RECEIVING_BYTES)<0)			// initialize 10 packets with 80 bytes each
	return -1;

	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
	if(csp_init(&csp_conf) < 0)			// initialize csp for the further use of ports and qfifo
	return -1;

	static csp_iface_t iface;
	csp_if_udp_init(&iface, IP );
	csp_rtable_set(0,0, &iface, CSP_NODE_MAC);

	// FOR TESTING
	// csp_buffer_init(1,1);
	// csp_packet_t * packet;
	// packet = csp_buffer_get(1);
	// packet->data[0] = 1;
	// packet->length = 1;
	// listen_in(packet->data, packet->length);

	csp_packet_t * packet;

	while(1){

		csp_buffer_init(1,MAX_SET_BYTES_REQUEST);
		csp_qfifo_t input;
		if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
			continue;
		}
		packet = input.packet;
	}

	// Everything for TCP
	sockfd = setup_socket();
	int case_id = packet->data[0];;

	switch(case_id)
	{
		case DISCOVERY_ID:
			printf("ID recv for discovery \n");
			discovery();
			break;
		case UPLOAD_ID:
			printf("ID recv for uploading \n");
			upload(packet->data);
			break;
		case DOWNLOAD_ID:
			printf("ID recv for downloading \n");
			download(packet->data);
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
			//upload_ddr(offset, size,data_buffer);
			break;
		case FPGA_ID:
			printf("Uploading to FPGA \n");
			//upload_fpga(offset, size,data_buffer);
			break;
		case FLASH_ID:
			printf("Uploading to FLASH \n");
			break;
		case RAM_ID:
			printf("Uploading to RAM \n");
			//upload_ram(offset, size,data_buffer);
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
			//send_datastream = download_stream_ddr(offset, size);
			break;
		case FPGA_ID:
			printf("Downloading from FPGA \n");
			//send_datastream = download_stream_fpga(offset, size);
			break;
		case FLASH_ID:
			printf("Downloading from FLASH \n");
			break;
		case RAM_ID:
			printf("Downloading from RAM \n");
			//send_datastream = download_stream_ram(offset, size);
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
