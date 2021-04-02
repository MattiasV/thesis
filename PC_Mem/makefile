CC= gcc
CFLAGS= -pthread -g -l json-c -I.
SRCFUDP = ./src/*.c ./src/rtable/csp_rtable_static.c ./src/arch/posix/*.c ./src/transport/*.c ./src/interfaces/csp_if_lo.c ./src/interfaces/csp_if_udp.c ./src/crypto/*.c
HDRF = ./src/interfaces/csp_if_can_pbuf.h

all: discovery upload download

# We gaan executable disovery maken
discovery: full_discovery.c MemoryAPI.c
	$(CC) -o discovery full_discovery.c MemoryAPI.c $(SRCFUDP) $(CFLAGS)

# We gaan executable upload maken
upload: full_upload.c MemoryAPI.c
	$(CC) -o upload full_upload.c MemoryAPI.c $(SRCFUDP) $(CFLAGS)

# We gaan executable upload maken
download: full_download.c MemoryAPI.c
	$(CC) -o download full_download.c MemoryAPI.c $(SRCFUDP) $(CFLAGS)

# Alles cleanen om project opnieuw te builden : type in console make clean
clean:
	rm discovery
	rm upload
	rm download
	
