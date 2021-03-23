#include "GSenseAPI.h"
#include "config.h"


int main(){

  if(csp_buffer_init(10, 100)<0)
  	printf("could not initialize buffer\n");

	csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
	if(csp_init(&csp_conf) < 0)
	printf("could not initialize csp\n");

	static csp_iface_t iface;
	csp_if_udp_init(&iface, "192.168.8.22" );
	csp_rtable_set(0,0, &iface, CSP_NODE_MAC);
  csp_packet_t * packet;
  packet = csp_buffer_get(100);
  packet->data[0] = 10;
  packet->length = 1;
  csp_sleep_ms(100);
  csp_if_udp_tx(&iface, packet, 1000);
  

  while(1){


  }


}
