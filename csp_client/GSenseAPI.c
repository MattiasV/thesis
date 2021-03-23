#include "GSenseAPI.h"
#include "jsonFunctions.h"

#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include <csp/interfaces/csp_if_udp.h>

// Size van de te zenden berichten
// 1 byte msg ID , 1 byte PAR_ID, (1-4) bytes value
#define SET_MSG_8 3
#define SET_MSG_32 6
#define SET_MSG_FL 6

/**
	download_list is the function that first asks the parameter list to the starterkit
	after receiving the paramater list it calls store_list to save it in JSON file
*/

void download_list()
{

	uint8_t data[1];
	int length;
	data[0] = DOWNLOAD_ID;
	length = sizeof(data);

	sendToServer(data, length, 0);

}

int load_list()
{
	csp_iface_t iface;
	csp_packet_t * packet;

	iface = init_udp();
	packet = csp_buffer_get(100);

	if (packet == NULL) {
		/* Could not get buffer element */
		printf("Failed to get buffer element\n");
		return 2;
	}
	packet->data[0] = REFRESH_ID;
	packet->length = 1;

	csp_if_udp_tx(&iface, packet, 1000);

	uint8_t rec_buffer;
	while(1){
		csp_qfifo_t input;
		if (csp_qfifo_read(&input) != CSP_ERR_NONE) {
			printf("reading\n");
			continue;
		}
		rec_buffer = input.packet->data[0];
		break;
	}

	if(rec_buffer == 1){
		printf("We moeten gaan refreshen\n");
		return 1;
	}
	else {
		printf("We moeten niet gaan refreshen \n");
		return 0;
	}
}

void sendToServer(uint8_t * data, int length, int amountOfIds){

	csp_iface_t iface;
	csp_packet_t * packet;

	iface = init_udp();
	packet = csp_buffer_get(100);

	int forloopsize = 0;
	switch(data[0]){
		case SET_ID:
			forloopsize = MAX_SET_BYTES;
			break;
		case GET_ID:
			forloopsize = MAX_GET_BYTES;
			break;
		case DOWNLOAD_ID:
			forloopsize = MAX_DOWNLOAD_BYTES;
			break;
		}

	for(int i = 0; i < forloopsize; i++){
		packet->data[i] = bytesToSend[i];
	}
	packet->length = length;
	csp_if_udp_tx(&iface, packet, 1000);

	/* receiving packet */
	csp_packet_t * rxpacket;
	while(1){
		csp_qfifo_t input;
		if (csp_qfifo_read(&input) != CSP_ERR_NONE) {
			continue;
		}
		rxpacket = input.packet;
		break;
	}
	returnedFromServer(rxpacket, amountOfIds);
}

void returnedFromServer(csp_packet_t * packet, int amountOfIds){

	uint8_t identifier = packet->data[0];

	uint8_t data[packet->length];
	for(int i = 1; i < packet->length; i++){
		data[i-1] = packet->data[i];
	}

	switch(data[0]){
		case SET_ID:
			setUpdated(amountOfIds);
			break;
		case GET_ID:
			addValues(data, sizeof(data));
			break;
		case DOWNLOAD_ID:
			store_list(data);
			break;
	}
}


csp_iface_t init_udp()
{
	if(csp_buffer_init(10, 100)<0)
	printf("could not initialize buffer\n");

	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
	if(csp_init(&csp_conf) < 0)
	printf("could not initialize csp\n");

	static csp_iface_t iface;
	csp_if_udp_init(&iface, IP );
	csp_rtable_set(0,0, &iface, CSP_NODE_MAC);

  return iface;
}
