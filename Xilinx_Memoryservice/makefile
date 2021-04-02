CC= gcc
CFLAGS= -pthread -g -l json-c -I.
SRCFUDP = ./src/*.c ./src/rtable/csp_rtable_static.c ./src/arch/posix/*.c ./src/transport/*.c ./src/interfaces/csp_if_lo.c ./src/interfaces/csp_if_udp.c ./src/crypto/*.c
HDRF = ./src/interfaces/csp_if_can_pbuf.h

all: memoryservice

# We gaan executable disovery maken
memoryservice: memoryservice.c memoryservice.h ddr_controller.c fpga_controller.c ram_controller.c controller.h
	$(CC) -o memoryservice memoryservice.c ddr_controller.c fpga_controller.c ram_controller.c controller.h $(SRCFUDP) $(CFLAGS)

# Alles cleanen om project opnieuw te builden : type in console make clean
clean:
	rm memoryservice
