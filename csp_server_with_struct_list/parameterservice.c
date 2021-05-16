#include "parameterservice.h"

int main(int argc, char* argv[])
{
	refresh = 1;

	// SETTING UP UDP CSP SERVER AND LISTEN FOR INCOMMING MSG
	csp_buffer_init(10, MAX_SET_BYTES_REQUEST);

  csp_conf_t csp_conf;
	csp_conf_get_defaults(&csp_conf);
	csp_conf.address = MY_ADDRESS;
  if(csp_init(&csp_conf) < 0)			// initialize csp for the further use of ports and qfifo
  	return -1;

	static csp_iface_t iface;
	csp_if_udp_init(&iface, IP );
  csp_rtable_set(0,0, &iface, CSP_NODE_MAC);


	while(1){

		csp_buffer_init(1,MAX_SET_BYTES_REQUEST);
    csp_qfifo_t input;
    if (csp_qfifo_read(&input) != CSP_ERR_NONE) {			// Waiting for incoming data
			continue;
		}
    csp_packet_t * packet;
		packet = input.packet;
		printf("packet->data[0] = %d\n", packet->data[0]);
		listen_in(&iface, packet->data, packet->length);
	}
	return 0;
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

/*
Kijken naar de MSG_ID en op basis van deze de bijhorende functie aanroepen
*/
void listen_in(csp_iface_t * iface, uint8_t * data, int length)
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
			//printf("ID received for downloading the parameterlist \n");
			send_parameter_list(iface);
			break;
		case SET_ID:
			//printf("We gaan een parameter instellen \n");
			set_parameter(iface, data, length);
			break;
		case GET_ID:
			//printf("We gaan een parameter opvragen with length: %d\n", length);
			get_parameter(iface, data, length);
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

/*
Deze functie gaat de parameter list omzetten naar een byte array en verstuurt deze naar de client met de MSG_ID ervoor
*/
void send_parameter_list(csp_iface_t * iface)
{
	//filling the union par_list
	for(int i = 0; i < DIFFERENT_PARAMETERS; i++){
		parameter_list_union.par_list[i] = parameterlist[i];
	}

	csp_buffer_init(1,sizeof(parameterlist)+1);
	csp_packet_t * packet = csp_buffer_get(sizeof(parameterlist)+1);

	//setting the message id
	packet->data[0] = DOWNLOAD_ID;

	// filling the packet with the byte array from the union
	for(int i = 0; i < sizeof(parameterlist); i++){
		packet->data[i+1] = parameter_list_union.par_list_bytes[i];
	}
	packet->length = sizeof(parameterlist) + 1;

	csp_if_udp_tx(iface, packet, TIMEOUT);
}

/*
Deze functie gaat de ID zoeken en wanneer hij een match vindt dan roept hij check_type_and_set_register op om de waarde wel echt weg te schrijven
Als alle data afgegaan is dan verstuurt hij een ACK terug naar de client
*/
void set_parameter(csp_iface_t * iface, uint8_t * data, int length)
{
	uint8_t par_id;
	int amount_of_paramters = sizeof(parameterlist)/sizeof(parameter_t);

	// for loopje met de length van het packet
	for(int i = 1; i < length;){

		par_id = data[i++];
		for(int j = 0; j < amount_of_paramters; j++){
			if(parameterlist[j].id == par_id){
				int index = i;
				index = check_type_and_set_register(i,j, data, parameterlist[j].datatype, parameterlist[j].offset);
				i = index;
			}
		}
	}

	csp_packet_t * packet;
	csp_buffer_init(1,1);
	packet = csp_buffer_get(1);
	packet->data[0] = data[0]; // packet->data[0] = SET_ID
	packet->length = 1;
  // printf("\n  Server sending: [%d]\n",packet->data[0]);
	// printf("  Format: [ACK]\n\n");

	csp_if_udp_tx(iface, packet, TIMEOUT);

}

/*
Deze functie zet de value die gedefinieerd zijn in set_parameter in de registers van de starterkit
Bij no error gaat hij deze ook updaten in de parameterlist zelf
*/
int check_type_and_set_register(int index, int parameterlist_index, uint8_t * data, int type, int offset){

	switch(type){

		case u8:
			if(setRegister_u8(offset, data[index])==1)
				parameterlist[parameterlist_index].value = data[index++];
			break;

		case i8:
			if(setRegister_i8(offset, data[index])==1)
				parameterlist[parameterlist_index].value = data[index++];
			break;

		case u16:
			fourBytesUnion.u8bytes[0] = data[index++];
			fourBytesUnion.u8bytes[1] = data[index++];
			if(setRegister_u16(offset, fourBytesUnion.u16bytes[0])==1)
				parameterlist[parameterlist_index].value = fourBytesUnion.u16bytes[0];
			break;

		case i16:
			fourBytesUnion.u8bytes[0] = data[index++];
			fourBytesUnion.i8bytes[1] = data[index++];
			if(setRegister_i16(offset, fourBytesUnion.i16bytes[0])==1)
				parameterlist[parameterlist_index].value = fourBytesUnion.i16bytes[0];
			break;

		case u32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[index++];
			}
			if(setRegister_u32(offset, fourBytesUnion.u32bytes)==1)
				parameterlist[parameterlist_index].value = fourBytesUnion.u32bytes;
			break;

		case i32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.i8bytes[i] = data[index++];
			}
			if(setRegister_i32(offset, fourBytesUnion.i32bytes)==1)
				parameterlist[parameterlist_index].value = fourBytesUnion.i32bytes;
			break;

		case f32:
			for(int i = 0; i < 4; i++){
				fourBytesUnion.u8bytes[i] = data[index++];
			}
			if(setRegister_float(offset, fourBytesUnion.fbytes)==1)
				parameterlist[parameterlist_index].value = fourBytesUnion.fbytes;
			break;
	}
	return index;
}

/*
Zoeken naar de ID en wanneer een match gevonden wordt de functie check_type_and_get_register aanroepen.
Deze functie returned de grootte van de data, uint8_t = 1, uint16_t = 2, uint32_t = 4, ...
Op basis van deze grootte gaat de packet->data gevuld worden.
Wanneer alle ID's zijn afgegaan verstuurt hij de byte array naar de client.
*/
void get_parameter(csp_iface_t * iface, uint8_t * data, int length)
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
	csp_if_udp_tx(iface, packet, TIMEOUT);
}

/*
Deze functie gaat de datatypes af en roept met een switch case de juiste functie aan om de juiste datatype te krijgen.
Hij vult de juiste waarde van de union om het wegschrijven in de packet->data te vergemakkelijken
*/
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
