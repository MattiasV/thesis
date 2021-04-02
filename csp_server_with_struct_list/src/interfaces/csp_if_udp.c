/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>
#include <csp/csp_interface.h>
#include <csp/arch/csp_thread.h>

struct sockaddr_in peer_addr = {0};

int csp_if_udp_tx(csp_iface_t * interface, csp_packet_t * packet, uint32_t timeout) {
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		return CSP_ERR_BUSY;
	}
	packet->id.ext = csp_hton32(packet->id.ext);
	peer_addr.sin_family = AF_INET;
	peer_addr.sin_port = htons(9600);
	int sent = sendto(sockfd, (void *) &packet->id, packet->length + 4, MSG_CONFIRM, (struct sockaddr *) &peer_addr, sizeof(peer_addr));
	csp_buffer_free(packet);
	close(sockfd);

	return CSP_ERR_NONE;
}

CSP_DEFINE_TASK(csp_if_udp_rx_task) {

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(9600);

	csp_iface_t * iface = param;

	while(1) {
		if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
			printf("UDP server waiting for port 9600\n");
			sleep(1);
			continue;
		}

		while(1) {
			uint8_t buffer[iface->mtu + 4];
			unsigned int peer_addr_len = sizeof(peer_addr);
			printf("Waiting for incomming data\n");
			int received_len = recvfrom(sockfd, (uint8_t *)buffer, iface->mtu + 4, MSG_WAITALL, (struct sockaddr *) &peer_addr, &peer_addr_len);
			/* Check for short */
			if (received_len < 4) {
				printf("Too short UDP packet");
				continue;
			}

			printf("UDP peer address: %s\n", inet_ntoa(peer_addr.sin_addr));

			/* Maak een packet klaar om alle data in te zetten */
			csp_packet_t * packet = csp_buffer_get(iface->mtu);
			if (packet == NULL)
				continue;

			/* putting values of the buffer in packet */
			memcpy(&packet->id, buffer, received_len);
			packet->length = received_len - 4;
			packet->id.ext = csp_ntoh32(packet->id.ext);

			csp_new_packet(packet, iface, NULL);


		}
	}

	return CSP_TASK_RETURN;

}

void csp_if_udp_init(csp_iface_t * iface, char * host) {

	if (inet_aton(host, &peer_addr.sin_addr) == 0) {
		printf("Unknown peer address %s\n", host);
	}

	/* Start thread */
	static csp_thread_handle_t handle_server;
	int ret = csp_thread_create(csp_if_udp_rx_task, "UDPS", 10000, iface, 0, &handle_server);

	/* MTU is datasize */
	iface->mtu = csp_buffer_datasize();
	/* Regsiter interface */
	iface->name = "UDP",
	iface->nexthop = csp_if_udp_tx,
	csp_iflist_add(iface);
}
