#include "GSenseAPI.h"
#include "json_functions.h"
#include "config.h"


void download_list()
{

	uint8_t data[1];
	int length;
	data[0] = DOWNLOAD_ID;
	length = sizeof(data);
	send_to_server(data, length, 0);

}


int load_list()
{

	uint8_t data[1];
	int length;
	data[0] = REFRESH_ID;
	length = sizeof(data);
	send_to_server(data, length, 0);
}


void check_refresh(uint8_t refresh){

	if(refresh == 1){
		printf("We moeten gaan refreshen\n");
	}
	else {
		printf("We moeten niet gaan refreshen \n");
	}
}


void send_to_server(uint8_t * data, int length, int amountOfIds){

	csp_iface_t iface;
	csp_packet_t * packet;

	iface = init_udp(length);
	csp_buffer_init(1,length);
	packet = csp_buffer_get(length);

	for(int i = 0; i < length; i++){
		packet->data[i] = data[i];
	}
	packet->length = length;
	//wait for rx thread to be at the reading line
	csp_sleep_ms(10);

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
	returned_from_server(rxpacket, amountOfIds);
}


void returned_from_server(csp_packet_t * packet, int amountOfIds){

	uint8_t identifier = packet->data[0];
	uint8_t data[packet->length - 1];
	for(int i = 1; i < packet->length; i++){
		data[i-1] = packet->data[i];
	}

	switch(identifier){
		case SET_ID:
			set_updated(amountOfIds);
			break;
		case GET_ID:
			add_values(data, sizeof(data));
			break;
		case DOWNLOAD_ID:
			store_list_from_bytes(data, packet->length -1);
			break;
		case REFRESH_ID:
			check_refresh(data[0]);
			break;
	}
}


csp_iface_t init_udp(int bytes)
{
	if(csp_buffer_init(1, bytes)<0)
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
