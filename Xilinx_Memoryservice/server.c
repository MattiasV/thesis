#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int setup_socket();

// TCP_USE
int setup_socket()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("TCP : socket creation failed...\n");
        exit(0);
    }
    else
		// niet perse nodig, terminal te vol
        //printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

	// zien dat we steeds met een cleane socket beginnen
	int true = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("TCP : socket bind failed...\n");
        exit(0);
    }
    else
		// niet perse nodig, terminal te vol
        // printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("TCP : Listen failed...\n");
        exit(0);
    }
    else
        printf("TCP : Server listening..\n");
    len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("TCP : server acccept failed...\n");
		exit(0);
	}
	else
	{
		printf("TCP : server acccept the client...\n");
		return connfd;
	}
}
