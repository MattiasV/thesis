#include "memoryservice.h"

int main(int argc, char *argv[])
{

	csp_iface_t * iface = init_udp(48);
	csp_buffer_init(1,4*sizeof(Memory));
	csp_packet_t * packet = csp_buffer_get(1);
	packet->data[0] = 0;
	packet->length = 1;
	check_packet(iface,packet);

	// while(1){
	//	csp_packet_t * packet = receiving_from_client();
	//	check_packet(iface,packet);
	// }

}

void check_packet(csp_iface_t * iface, csp_packet_t * packet){

	int case_id = packet->data[0];
	csp_buffer_free(packet);

	switch(case_id)
	{
		case DISCOVERY_ID:
			printf("ID recv for discovery \n");
			discovery(iface);
			break;
		case UPLOAD_ID:
			printf("ID recv for uploading \n");
			upload(iface,packet->data);
			break;
		case DOWNLOAD_ID:
			printf("ID recv for downloading \n");
			download(iface,packet->data);
			break;
		default:
			printf("Default case \n");
			break;
	}

}

void discovery(csp_iface_t * iface)
{

	for(int i = 0; i < sizeof(memory_list)/sizeof(Memory); i++){
		memory_list_union.mem_list[i] = memory_list[i];
	}
	printf("sending to client\n");
	send_to_client(iface, memory_list_union.mem_list_bytes, sizeof(memory_list));

}


void upload(csp_iface_t * iface, uint8_t * buff)
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

void download(csp_iface_t * iface, uint8_t * buff)
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

	send_to_client(iface, send_buffer, size);

}


csp_iface_t * init_udp(int length){

	if(csp_buffer_init(1, 4*sizeof(Memory))<0)
	return NULL;

	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
	if(csp_init(&csp_conf) < 0)			// initialize csp for the further use of ports and qfifo
	return NULL;

	static csp_iface_t udp_iface;
	csp_if_udp_init(&udp_iface, DEST_IP );
	csp_rtable_set(0,0, &udp_iface, CSP_NODE_MAC);

	return &udp_iface;

}


csp_packet_t * receiving_from_client(){

	csp_packet_t * recv_packet;

	while(1){

		csp_qfifo_t input;
		if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
			continue;
		}
		recv_packet = input.packet;
	}

	return recv_packet;

}


void send_to_client(csp_iface_t * iface, uint8_t * data, int length){

	csp_buffer_init(1,length);
	csp_packet_t * packet = csp_buffer_get(length);
	printf("length: %d\n", length);
	for(int i = 0; i < length; i++){
		packet->data[i] = data[i];
	}
	packet->length = length;

	csp_if_udp_tx(iface, packet, TIMEOUT);
	printf("sent to client\n");

}
