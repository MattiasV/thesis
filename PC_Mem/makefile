CC= gcc
CFLAGS= -I.

all: discovery upload download

# We gaan executable disovery maken
discovery: full_discovery.c MemoryAPI.c
	$(CC) -o discovery full_discovery.c MemoryAPI.c $(CFLAGS)

# We gaan executable upload maken
upload: full_upload.c MemoryAPI.c
	$(CC) -o upload full_upload.c MemoryAPI.c $(CFLAGS)

# We gaan executable upload maken
download: full_download.c MemoryAPI.c
	$(CC) -o download full_download.c MemoryAPI.c $(CFLAGS)

# Alles cleanen om project opnieuw te builden : type in console make clean
clean:
	rm discovery
	rm upload
	rm download
