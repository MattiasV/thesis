#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

/*
 * 				FPGA PART
 */

#define FPGA_MAP_SIZE 0x10000000

void upload_fpga(uint32_t offset, uint32_t data_size, uint8_t data_stream[]);
uint8_t * download_stream_fpga(uint32_t offset, uint32_t size);
int fpga_open_uio();

void print_fpga(uint32_t offset, uint32_t size);


/*
 * 				DDR PART
 */

#define DDR_MAP_SIZE 0x1000

uint8_t * download_stream_ddr(uint32_t offset, uint32_t size);
void upload_ddr(uint32_t offset, uint32_t data_size, uint8_t data_stream[]);
int ddr_open_uio();

void print_ddr(uint32_t offset, uint32_t size);


/*
 * 				RAM PART
 */

#define RAM_MAP_SIZE 0x1000

void upload_ram(uint32_t offset, uint32_t data_size, uint8_t data_stream[]);
uint8_t * download_stream_ram(uint32_t offset, uint32_t size);
int ram_open_uio();

void print_ram(uint32_t offset, uint32_t size);

#endif /* CONTROLLER_H */

