#include "parameterservice.h"

int main(int argc, char* argv[])
{
	refresh = 1;

	// SETTING UP UDP CSP SERVER AND LISTEN FOR INCOMMING MSG

	if(csp_buffer_init(1, 80)<0)			// initialize 10 packets with 80 bytes each
  return -1;

  csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
  if(csp_init(&csp_conf) < 0)			// initialize csp for the further use of ports and qfifo
  return -1;

	static csp_iface_t iface;
	csp_if_udp_init(&iface, IP );
  csp_rtable_set(0,0, &iface, CSP_NODE_MAC);

  while(1) {

		csp_buffer_init(1,MAX_PACKET_SIZE);
    csp_qfifo_t input;
    if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
			continue;
		}
    csp_packet_t * packet;
		packet = input.packet;

		listen_in(packet->data, packet->length);
	}
}

// Function to put parameters list in a byte array
void convert_parameter_list()
{

}

int get_parameter_list_size()
{
	int list_size = sizeof(parameterlist);
	csp_buffer_init(1,1);
	csp_packet_t * packet;
	packet = csp_buffer_get(1);
	packet->data[0] = list_size;
	packet->length = 1;
	printf("De grootte van de parameterlist bedraagt %d bytes \n", list_size);

	csp_if_udp_tx(&iface, packet, TIMEOUT);
	return list_size;
}

// Function designed for chat between client and server.
void listen_in(uint8_t * data, int length)
{
	// We krijgen altijd maar max een buffer van 4-6 bytes binnen

	int par_id;
	int case_id;
	case_id = data[0];
	switch(case_id)
	{
		case REFRESH_ID:
			printf("ID received to ask for paramterlist \n");
			send_refresh();
			break;
		case DOWNLOAD_ID:
			printf("ID received for downloading the parameterlist \n");
			send_parameter_list();
			break;
		case SET_ID:
			printf("We gaan een parameter instellen \n");
			data = &data[1];
			set_parameter(data, length-1);
			break;
		case GET_ID:
			printf("We gaan een parameter opvragen \n");
			data = &data[1];
			get_parameter(data, length-1);
			break;
		case SIZE_ID:
			printf("We gaan de grootte van de parameterlijst opvragen \n");
			get_parameter_list_size();
			break;
		default:
			printf("Default case, exit programm \n");
			break;
	}
}

// Send refresh to client
void send_refresh()
{
	csp_buffer_init(1,1);
	csp_packet_t * packet;
	packet = csp_buffer_get(1);
	packet->data[0] = refresh;
	packet->length = 1;
	csp_if_udp_tx(&iface, packet, TIMEOUT);
}

// Send parameter list to client
void send_parameter_list()
{
	convert_parameter_list();

}

void set_parameter(uint8_t * data, int length)
{
	uint8_t par_id;
	int amount_of_paramters = sizeof(parameterlist)/sizeof(parameter_t);

	// for loopje met de length van het packet
	for(int i = 0; i <= length;){

		par_id = data[i];
		printf("par_id: %d\n", par_id);

		for(int j = 0; j < amount_of_paramters; j++){

			if(parameterlist[j].id == par_id){
				i++; //set the index on the first value byte of data[]
				check_type_and_set_register(&i, data, parameterlist[j].datatype, parameterlist[j].offset);
			}
		}
	}

	csp_packet_t * packet;
	packet = csp_buffer_get(1);
	packet->data[0] = data[0]; // packet->data[0] = SET_ID
	packet->length = 1;
	csp_if_udp_tx(&iface, packet, TIMEOUT);

}

void check_type_and_set_register(int * index, uint8_t * data, int type, int offset){
	switch(type){
		case u8:
			setRegister_8(offset, data[*index]);
			*index++; //set index on next ID
			break;
		case i8:
			setRegister_8(offset, data[*index]);
			*index++;
			break;
		case u16:
			fourBytesUnion.u8bytes[0] = data[*index];
			*index ++;
			fourBytesUnion.u8bytes[1] = data[*index];
			*index++;
			setRegister_16(offset, fourBytesUnion.u16bytes);
			break;
		case i16:
			fourBytesUnion.u8bytes[0] = data[*index];
			*index ++;
			fourBytesUnion.i8bytes[1] = data[*index];
			*index++;
			setRegister_16(offset, fourBytesUnion.i16bytes);
			break;
		case u32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[*index];
				*index++;
			}
			setRegister_32(offset, fourBytesUnion.u32bytes);
			break;
		case i32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.i8bytes[i] = data[*index];
				*index++;
			}
			setRegister_32(offset, fourBytesUnion.i32bytes);
			break;
		case f32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[*index];
				*index++;
			}
			setRegister_32(offset, fourBytesUnion.ufbytes);
			break;
	}
}

// Functie om de parameter te gaan lezen van de zynq
void get_parameter(uint8_t * data, int length)
{
	uint8_t recv8;
	uint8_t sendbuf8[1];
	uint32_t recv32;
	uint32_t sendbuf32[1];
	float recvfl;
	float sendbuffl[1];

	csp_buffer_init(1,(1+4)*length; //1 for each ID and 4 for the max amount of bytes in a value
	csp_packet_t * packet;
	packet = csp_buffer_get((1+4)*length);

	int amount_of_paramters = sizeof(parameterlist)/sizeof(parameter_t);

	int index = 0;

	for(int i = 0; i < length; i++){
		for(int j = 0; j < amount_of_paramters; j++){

			if(parameterlist[j] == data[i]){
				int sizereturnedvalue = check_type_and_get_register(parameterlist[j].datatype, parameterlist[j].offset);
				for(int k = 0; k < sizereturnedvalue; k++){
					packet->data[index] = fourBytesUnion.u8bytes[k];
					index++;
				}
			}
		}
	}
	packet->length = index;
	csp_if_udp_tx(&iface, packet, TIMEOUT);

}

int check_type_and_get_register(int datatype, int offset){
	switch(type){
		case u8:
			return getRegister_8(offset);
		case i8:
			return getRegister_8(offset);
		case u16:
			return getRegister_16(offset);
		case i16:
			return getRegister_16(offset);
		case u32:
			return getRegister_32(offset);
		case i32:
			return getRegister_32(offset);
		case f32:
			return getRegister_32(offset);
	}
}
