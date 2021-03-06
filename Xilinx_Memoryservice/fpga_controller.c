#include "controller.h"

void upload_fpga(uint32_t offset, uint32_t data_size, uint8_t data_stream[])
{
	// Part 1 : open the fpga register with the uio driver + mmap
	int fpga_fd;
	unsigned char* write_addr;
	fpga_fd = fpga_open_uio();
	write_addr = (unsigned char *) mmap(NULL, FPGA_MAP_SIZE, PROT_WRITE, MAP_SHARED, fpga_fd,0);
	if(write_addr == NULL) { printf("Failed to map memory \n");}

	// Part 2 : write the data to the fpga register
	for(uint32_t i = 0; i < data_size; i++)
	{
		write_addr[offset + i] = data_stream[i];
	}

	// Part 3 : Unmap the FPGA
	munmap(write_addr,FPGA_MAP_SIZE);
}

uint8_t * download_stream_fpga(uint32_t offset, uint32_t size)
{
	uint8_t *send_buffer = (uint8_t *) malloc(sizeof(uint8_t)*size);

	// Part 1 : open the fpga register with the uio driver + mmap
	int fpga_fd;
	void *address, *virt_addr;
	unsigned long read_result;
	uint8_t result;

	fpga_fd = fpga_open_uio();
	address = (unsigned char *) mmap(NULL, FPGA_MAP_SIZE, PROT_READ, MAP_SHARED, fpga_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	// Part 2 : read the values from the register + put them into the send_buffer
	for(uint32_t i = 0; i < size; i++)
	{
		virt_addr = address + offset + i;
		read_result = *((unsigned char *) virt_addr);
		result = read_result;
		send_buffer[i] = result;
	}

	// Part 3 : Unmap the FPGA
	munmap(address,FPGA_MAP_SIZE);

	// To check buffer try the : print_send_buffer(send_buffer,size)
	return send_buffer;
}

int fpga_open_uio()
{
	int fd = open("/dev/uio1", O_RDWR);
	if(fd == 0){ printf("Cannot open FPGA UIO file \n");}
	return fd;
}

void print_fpga(uint32_t offset, uint32_t size)
{
	// PART TO GET THE VALUES UIT OF THE FPGA REGISTER
	int fpga_fd;
	void *address, *virt_addr;
	unsigned long read_result;
	uint8_t result;

	fpga_fd = fpga_open_uio();
	address = (unsigned char *) mmap(NULL, FPGA_MAP_SIZE, PROT_READ, MAP_SHARED, fpga_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	for(uint32_t i = 0; i < size; i++)
	{
		virt_addr = address + offset + i;
		read_result = *((unsigned char *) virt_addr);
		result = read_result;
		printf("Value of the file at offset 256 + %d  is : %d \n", i, result);
	}

	munmap(address,FPGA_MAP_SIZE);
}

