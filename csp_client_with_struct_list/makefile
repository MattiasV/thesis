CC=gcc
CFLAGS= -pthread -g -l json-c -I.
SRCFUDP = ./src/*.c ./src/rtable/csp_rtable_static.c ./src/arch/posix/*.c ./src/transport/*.c ./src/interfaces/csp_if_lo.c ./src/interfaces/csp_if_udp.c ./src/crypto/*.c
HDRF = ./src/interfaces/csp_if_can_pbuf.h config.h json_functions.h GSenseAPI.h

all: print_parameter_list set_parameter get_parameter

# We gaan executable print_parameter_list maken
print_parameter_list: full_print_parameter_list.c GSenseAPI.c json_functions.c $(HDRF)
	$(CC) -o print_parameter_list full_print_parameter_list.c GSenseAPI.c json_functions.c $(SRCFUDP) $(CFLAGS)

# We gaan executable set_parameter maken
set_parameter: full_set_parameter.c GSenseAPI.c json_functions.c $(HDRF)
	$(CC) -o set_parameter full_set_parameter.c GSenseAPI.c json_functions.c $(SRCFUDP) $(CFLAGS)

# We gaan executable get_parameter maken
get_parameter: full_get_parameter.c GSenseAPI.c json_functions.c $(HDRF)
	$(CC) -o get_parameter full_get_parameter.c GSenseAPI.c json_functions.c $(SRCFUDP) $(CFLAGS)

jsonFunctions: full_get_parameter.c json_functions.c GSenseAPI.c $(HDRF)
	$(CC) -o jsonFunctions full_get_parameter.c GSenseAPI.c json_functions.c $(SRCFUDP) $(CFLAGS)

test: test.c ./src/interfaces/csp_if_udp.c $(HDRF)
	$(CC) -o test test.c $(SRCFUDP) -pthread $(CFLAGS)

# Alles cleanen om project opnieuw te builden : type in console make clean
clean:
	rm print_parameter_list
	rm set_parameter
	rm get_parameter
