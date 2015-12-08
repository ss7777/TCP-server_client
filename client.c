#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA "HELLOW WORLD OF SOCKET"

int main(int argc, char *argv[])

{

	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buff[1024];

	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}


	server.sin_family = AF_INET;
	
	hp = gethostbyname (argv[1]);
	if(hp == 0)
	{
		perror("gethostbyname failed");
		close(sock);
		exit(1);
	}

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(5000);


	if(connect(sock, (struct sockaddr *) &server, sizeof(server))<0)

	{
		perror("connect failed");
		close(sock);
		exit(1);
	}


	if(send(sock, DATA, sizeof(DATA), 0) >0)

	{
		//perror("send failed");
		//exit(1);

	printf("sent %s\n", DATA);
	//close(sock);	
	//int read_size = recv(sock, buff , sizeof(buff) , 0);
	//puts(buff);

	}
int n;	
while (  n = recv(sock, buff , sizeof(buff) , 0) > 0)
    {
        //recvBuff[n] = 0;
        //if(fputs(recvBuff, stdout) == EOF)
        //{
        //    printf("\n Error : Fputs error\n");
        //}
       puts(buff); 
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

	
 	return 0;
 	
}
