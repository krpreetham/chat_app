//client side program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error (const char *msg)
{
	perror(msg);
	exit(1);
}

struct emp_details
{
	int emp_id;
	char emp_name[255];
};

int main(int argc, char * argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	struct emp_details temp_emp;

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

	while(1)
	{
		bzero((char *) &temp_emp, sizeof(temp_emp));
		
		printf("Enter employee name :");
//		fgets(temp_emp.emp_name, 255, stdin);
//		gets(temp_emp.emp_name);
		scanf("%s", temp_emp.emp_name);
		printf("Enter employee id :");
		scanf("%d", &temp_emp.emp_id);
//		temp_emp.emp_name =getchar();
		
//		temp_emp.emp_id=21;
//		temp_emp.emp_name = "preetham";
		n = send(sockfd,(void *) &temp_emp , sizeof(temp_emp), 0);
		if(n < 0)
			error("Error on sending");
		bzero((char *) &temp_emp, sizeof(temp_emp));
		n = recv(sockfd,(struct emp_details *) &temp_emp , sizeof(struct emp_details), 0);
		if(n < 0)
			error("Error on reading.");
		int i = strncmp("null", temp_emp.emp_name, 3);
		if(i == 0)
			break;
		printf("--------------Data from Server---------------\n");
		printf("Employee name -> %s \nEmployee ID -> %d\n", temp_emp.emp_name, temp_emp.emp_id);
		printf("------------------------------------------\n");
		
		
	}

	close(sockfd);
	return 0;
}

