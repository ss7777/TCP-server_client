#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>    //write
#include <arpa/inet.h> //inet_addr

//the thread function
void *connection_handler(void *);


int main(int argc, char *argv[])

{
	/* Variables */
	int sock, mysock, connection;
	struct sockaddr_in server, client;
	

	
	char buff[1024];
	int rval;	
	

	pthread_t thread_id;


	/* Creat Socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Failed to creat Socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000)		;

	/* Call Bind */

	if(bind(sock, (struct sockaddr *)&server, sizeof(server))<0)
	{
		perror("Bind Failed");
	}

	/* Listen */

	listen(sock,5);

	puts("ready for listening and Waiting for incoming connections\n");

	/* Accept */
	while(1)
	{	
		//connection = (server *) malloc(sizeof(struct sockaddr_in));
		socklen_t connection = sizeof(client);

		//mysock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&connection);
		mysock = accept(sock, (struct sockaddr *)&client, &connection);

		puts("Connection accepted");

		if(mysock == -1)
		{
			perror("accept failed");		
		}
		
		else
		{	/* start a new thread but do not wait for it */
			pthread_create(&thread_id, NULL, connection_handler, (void *)&mysock);
			pthread_detach(thread_id);
		}

	}

	
	return 0;

}


void *connection_handler(void *mysock)
{	puts("Handler assigned");
    //Get the socket descriptor
    int read_size;
    int sock_desk = *(int*)mysock;
    char *message , client_message[2000];
     
    

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock_desk , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    write(sock_desk , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock_desk , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
		client_message[read_size] = '\0';

		puts(client_message);
		//Send the message back to client
        write(sock_desk , client_message , strlen(client_message));
		
		//clear the message buffer
		memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return 0;

}	
