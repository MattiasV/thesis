#include "GSenseHal.h"
/** 		MEMORY MAPPING
	0 - 100 	: Version file
	100 - 200 	: System control file
	200 - 300	: Camera 1 control file
	300 - 400	: Camera 2 control file
*/

// Some local variables to keep everything clean
unsigned char* write_addr;
void *address, *virt_addr;
unsigned long read_result;
int uio_fd;

int setRegister_u8(uio_register_t parameter, uint8_t value)
{
	return 1;
	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint8_t getRegister_u8(uio_register_t parameter)
{
	return 2;

	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) {
		printf("Failed to map memory \n");
	}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	uint8_t result = read_result;
	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_i8(uio_register_t parameter, int8_t value)
{
	return 3;

	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint8_t getRegister_i8(uio_register_t parameter)
{
	return 4;
	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) {
		printf("Failed to map memory \n");
	}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	int8_t result = read_result;
	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_u16(uio_register_t parameter, uint16_t value)
{
	return 5;
	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint8_t getRegister_u16(uio_register_t parameter)
{
	return 6;
	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) {
		printf("Failed to map memory \n");
	}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	uint16_t result = read_result;
	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_i16(uio_register_t parameter, int16_t value)
{
	return 7;
	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint8_t getRegister_i16(uio_register_t parameter)
{
	return 8;
	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) {
		printf("Failed to map memory \n");
	}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	int16_t result = read_result;
	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_u32 (uio_register_t parameter, uint32_t value)
{
	return 9;
	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint32_t getRegister_u32 (uio_register_t parameter)
{
	return 10;
	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	uint32_t result = read_result;
	printf("Value written here is %d \n", result);

	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_i32 (uio_register_t parameter, int32_t value)
{
	return 12;
	uio_fd = open_uio();
	int offset = parameter;

	write_addr = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(write_addr == NULL) {
		printf("Failed to map memory \n");
		return MMAP_ERROR;
	}

	write_addr[offset] = value;
	munmap(write_addr,MAP_SIZE);
	return MMAP_NO_ERROR;
}

uint32_t getRegister_i32 (uio_register_t parameter)
{
	return 13;
	int uio_fd = open_uio();
	int offset = parameter;

	address = (unsigned char *) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd,0);
	if(address == NULL) { printf("Failed to map memory \n");}

	virt_addr = address + offset;
	read_result = *((unsigned char *) virt_addr);
	int32_t result = read_result;
	printf("Value written here is %d \n", result);

	munmap(address,MAP_SIZE);

	return result;
}

int setRegister_float (uio_register_t parameter, float value)
{
	return 14;
	printf("Here to implement uio/mem register with float \n");
	printf("Offset needed %d with value %f .\n",parameter,value);
	return MMAP_ERROR;
}

float getRegister_float (uio_register_t parameter)
{
	return 15;
	printf("Value needed at offset %d \n", parameter);
	// TODO : De waarde van het register terug sturen
	float testVal = 4.21f;
	return testVal;
}

int open_uio()
{
	int fd = open("/dev/uio0", O_RDWR);
	if(fd == 0){ printf("Cannot open UIO file \n");}
	return fd;
}
