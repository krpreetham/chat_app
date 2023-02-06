//client side program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

char rev_msg[255], send_msg[255];
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;

void error (const char *msg)
{
	perror(msg);
	exit(1);
}

void* read_msg(void *arg)
{
	while(1)
	{
		bzero(rev_msg, 255);
		n = read(sockfd, rev_msg, 255);
		if(n < 0)
			error("Error on reading.");
		printf("Client : %s\n", rev_msg);
		int i = strncmp("Bye", rev_msg, 3);
		if(i == 0)
			break;
	}
} 

void* write_msg(void *arg)
{
	while(1)
	{
		bzero(send_msg, 255);
		fgets(send_msg, 255, stdin);
		n = write(sockfd, send_msg, sizeof(send_msg));
		if(n < 0)
			error("Error in writing.");
		int i = strncmp("Bye", rev_msg, 3);
		if(i == 0)
			break;
	}
}

int main(int argc, char * argv[])
{
/*	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
*/
	char buffer[255];
	if(argc <3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
		error("ERROR opeing socket");

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr , "Error , no such host");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Connection Failed");
	printf("Client Side\n");
	printf("========================================\n");
	
	pthread_t t1, t2;
	pthread_create(&t1,NULL,read_msg,NULL);
	pthread_create(&t2,NULL,write_msg,NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

/*	while(1)
	{
		printf("Enter msg here for server: ");
		bzero(buffer , 255);
		fgets(buffer , 255 , stdin);
		n = write(sockfd , buffer , strlen(buffer));
		if(n < 0)
			error("Error on writing");
		bzero(buffer , 255);
		n = read(sockfd , buffer , 255);
		if(n <0)
			error("Error on reading");
		printf("Server : %s", buffer);
		int z = strncmp("Bye", buffer , 3);
		if(z==0)
			break;
	}
*/
	close(sockfd);
	return 0;
}

