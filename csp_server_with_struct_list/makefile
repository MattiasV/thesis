CC= gcc
CFLAGS= -pthread -g -l json-c -I.
SRCFUDP = ./src/*.c ./src/rtable/csp_rtable_static.c ./src/arch/posix/*.c ./src/transport/*.c ./src/interfaces/csp_if_lo.c ./src/interfaces/csp_if_udp.c ./src/crypto/*.c
HDRF = ./src/interfaces/csp_if_can_pbuf.h

all: parameter_service

# We gaan executable print_parameter_list maken
parameter_service: parameterservice.c GSenseHal.c parameterservice.h GSenseHal.h parameterlist.h $(SRCFUDP)
	$(CC) -o parameter_service parameterservice.c GSenseHal.c $(SRCFUDP) $(HDRF) $(CFLAGS)

test: test.c ./src/interfaces/csp_if_udp.c
	$(CC) -o test test.c $(CFLAGS)

clean:
	rm parameter_service
