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
		csp_buffer_init(1,80);

    csp_qfifo_t input;
    if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
			continue;
		}
    csp_packet_t * packet;
		packet = input.packet;
		listen_in(packet->data, packet->length);
		printf("buffer size remaining: %d\n", csp_buffer_remaining());

	}
}

// Function to put parameters list in a byte array
void convert_parameter_list()
{

}

int get_parameter_list_size()
{
	int list_size = sizeof(parameterlist);
	csp_buffer_init(1,sizeof(list_size));
	csp_packet_t * packet;
	packet = csp_buffer_get(sizeof(list_size));
	for(int i = 0; i < sizeof(list_size); i++){
		packet->data[0] = list_size;
	}
	packet->length = sizeof(list_size);
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
			set_parameter(data, length);
			break;
		case GET_ID:
			printf("We gaan een parameter opvragen \n");
			get_parameter(data, length);
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
	for(int i = 1; i < length;){

		par_id = data[i];
		printf("par_id: %d\n", par_id);
		for(int j = 0; j < amount_of_paramters; j++){
			if(parameterlist[j].id == par_id){
				i++; //set the index on the first value byte of data[]
				printf("j: %d\n",j);
				check_type_and_set_register(&i, data, parameterlist[j].datatype, parameterlist[j].offset);
			}
		}
	}

	csp_packet_t * packet;
	csp_buffer_init(1,1);
	packet = csp_buffer_get(1);
	packet->data[0] = data[0]; // packet->data[0] = SET_ID
	packet->length = 1;
	printf("packet->data: %d\n", packet->data[0]);
	//csp_if_udp_tx(&iface, packet, TIMEOUT);

}

void check_type_and_set_register(int * index, uint8_t * data, int type, int offset){
	switch(type){
		case u8:
			setRegister_u8(offset, data[*index]);
			*index++; //set index on next ID
			break;
		case i8:
			setRegister_i8(offset, data[*index]);
			*index++;
			break;
		case u16:
			fourBytesUnion.u8bytes[0] = data[*index];
			*index ++;
			fourBytesUnion.u8bytes[1] = data[*index];
			*index++;
			setRegister_u16(offset, fourBytesUnion.u16bytes[0]);
			break;
		case i16:
			fourBytesUnion.u8bytes[0] = data[*index];
			*index ++;
			fourBytesUnion.i8bytes[1] = data[*index];
			*index++;
			setRegister_i16(offset, fourBytesUnion.i16bytes[0]);
			break;
		case u32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[*index];
				*index++;
			}
			setRegister_u32(offset, fourBytesUnion.u32bytes);
			break;
		case i32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.i8bytes[i] = data[*index];
				*index++;
			}
			setRegister_i32(offset, fourBytesUnion.i32bytes);
			break;
		case f32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[*index];
				printf("index: %d\n", *index);
				*index++;
			}
			setRegister_float(offset, fourBytesUnion.fbytes);
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

	csp_buffer_init(1,(1+4)*length); //1 for each ID and 4 for the max amount of bytes in a value
	csp_packet_t * packet;
	packet = csp_buffer_get((1+4)*length);

	int amount_of_paramters = sizeof(parameterlist)/sizeof(parameter_t);

	packet->data[0] = data[0];
	int index = 1;
	for(int i = 1; i < length; i++){
		for(int j = 0; j < amount_of_paramters; j++){

			if(parameterlist[j].id == data[i]){
				packet->data[index++] = data[i]; //set the ID in the data
				int sizereturnedvalue = check_type_and_get_register(parameterlist[j].datatype, parameterlist[j].offset);
				for(int k = 0; k < sizereturnedvalue; k++){
					packet->data[index++] = fourBytesUnion.u8bytes[k];
				}
				fourBytesUnion.u32bytes = 0; //empty the fourBytesUnion memory
			}
		}
	}
	packet->length = index;
	for(int i = 0; i < index; i++){
		printf("packet->data[%d]: %d\n", i, packet->data[i]);
	}
	//csp_if_udp_tx(&iface, packet, TIMEOUT);

}

int check_type_and_get_register(int datatype, int offset){
	switch(datatype){
		case u8:
			fourBytesUnion.u8bytes[0] =  getRegister_u8(offset);
			return 1;
		case i8:
			fourBytesUnion.i8bytes[0] = getRegister_i8(offset);
			return 1;
		case u16:
			fourBytesUnion.u16bytes[0] = getRegister_u16(offset);
			return 2;
		case i16:
			fourBytesUnion.i16bytes[0] = getRegister_i16(offset);
			return 2;
		case u32:
			fourBytesUnion.u32bytes = getRegister_u32(offset);
			return 4;
		case i32:
			fourBytesUnion.i32bytes = getRegister_i32(offset);
			return 4;
		case f32:
			fourBytesUnion.fbytes = getRegister_float(offset);
			return 4;
	}
}
