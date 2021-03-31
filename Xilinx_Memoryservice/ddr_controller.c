#include "controller.h"

void upload_ddr(uint32_t offset, uint32_t data_size, uint8_t data_stream[])
{
	// Part 1 : open the DDR register with the uio driver + mmap
	int ddr_fd;
	unsigned char* write_addr;
	ddr_fd = ddr_open_uio();
	write_addr = (unsigned char *) mmap(NULL, DDR_MAP_SIZE, PROT_WRITE, MAP_SHARED, ddr_fd,0);
	if(write_addr == NULL) { printf("Failed to map memory \n");}

	// Part 2 : write the data to the DDR register
	for(uint32_t i = 0; i < data_size; i++)
	{
		write_addr[offset + i] = data_stream[i];
	}

	// Part 3 : Unmap the DDR
	munmap(write_addr,DDR_MAP_SIZE);
}

uint8_t * download_stream_ddr(uint32_t offset, uint32_t size)
{
	uint8_t *send_buffer = (uint8_t *) malloc(sizeof(uint8_t)*size);

	// Part 1 : open the DDR register with the uio driver + mmap
	int ddr_fd;
	void *address, *virt_addr;
	unsigned long read_result;
	uint8_t result;

	ddr_fd = ddr_open_uio();
	address = (unsigned char *) mmap(NULL, DDR_MAP_SIZE, PROT_READ, MAP_SHARED, ddr_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	// Part 2 : read the values from the register + put them into the send_buffer
	for(uint32_t i = 0; i < size; i++)
	{
		virt_addr = address + offset + i;
		read_result = *((unsigned char *) virt_addr);
		result = read_result;
		send_buffer[i] = result;
	}

	// Part 3 : Unmap the DDR
	munmap(address,DDR_MAP_SIZE);
	return send_buffer;
}

int ddr_open_uio()
{
	int fd = open("/dev/uio2", O_RDWR);
	if(fd == 0){ printf("Cannot open DDR UIO file \n");}
	return fd;
}

void print_ddr(uint32_t offset, uint32_t size)
{
	// PART TO GET THE VALUES UIT OF THE FPGA REGISTER
	int ddr_fd;
	void *address, *virt_addr;
	unsigned long read_result;
	uint8_t result;

	ddr_fd = ddr_open_uio();
	address = (unsigned char *) mmap(NULL, DDR_MAP_SIZE, PROT_READ, MAP_SHARED, ddr_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	for(uint32_t i = 0; i < size; i++)
	{
		virt_addr = address + offset + i;
		read_result = *((unsigned char *) virt_addr);
		result = read_result;
		printf("Value of the file at offset 256 + %d  is : %d \n", i, result);
	}

	munmap(address,DDR_MAP_SIZE);
}

