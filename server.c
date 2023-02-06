//server side program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
	if(argc < 2)
	{
		fprintf(stderr, "Port No not provided. Program Terminated\n");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	struct emp_details temp_emp;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("Error opening Socket.");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		error("Binding Failed");
	}
	
	printf("Server Side\n");
        printf("=========================================\n");
	listen(sockfd, 2);

	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		error("Error on Accept");
	}
	
	while(1)
	{
		bzero((char *) &temp_emp, sizeof(temp_emp));
		n = recv(newsockfd,(struct emp_details *) &temp_emp , sizeof(struct emp_details), 0);
		if(n < 0)
			error("Error on reading.");
		printf("--------------Data from Client----------------\n");
		printf("Employee name -> %s\nEmployee ID -> %d\n", temp_emp.emp_name, temp_emp.emp_id);
		printf("------------------------------------------\n");
		bzero((char *) &temp_emp, sizeof(temp_emp));
		
		printf("Enter employee name :");
//		fgets(temp_emp.emp_name, 255, stdin);
//		gets(temp_emp.emp_name);
		scanf("%s", temp_emp.emp_name);
		printf("Enter employee id :");
		scanf("%d", &temp_emp.emp_id);
//		temp_emp.emp_name =getchar();

//		temp_emp.emp_id = 22;
//		temp_emp.emp_name = "kr";
		n = send(newsockfd,(void *) &temp_emp , sizeof(temp_emp), 0);
		if(n < 0)
			error("Error in writing.");
		int i = strncmp("null", temp_emp.emp_name, 3);
		if(i == 0)
			break;
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
