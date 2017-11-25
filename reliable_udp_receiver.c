// reliable udp sender
// created 19 november 2017
// jim bui and luis franco

#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#include <stdlib.h>         // Needed for exit()
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff

int main()
{
	char receivedFile[] = "fileReceive.dat" ; // put received data here.
	int portNum = 6008 ; // confirm.
	int maxSize = 10 ;
	int options = 0 ;

	int retcode = recvFile(&receivedFile , portNum , maxSize , options) ; 
}

int recvFile(char *receivedFile , int portNum , int maxSize , int options)
{
	int receiver_s ;
	struct sockaddr_in receiver_addr ;
	struct sockaddr_in sender_addr ;
	int addr_len ;
	// char send_buffer[4096] ;
	char receive_buffer[4096] ;
	int retcode ;

	// create socket.

	receiver_s = socket(AF_INET , SOCK_DGRAM , 0) ;

	if (receiver_s < 0)
	{
		printf("socket() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// fill socket data.  confirm if this is destination information.

	receiver_addr.sin_family = AF_INET ; // address family to use.
	receiver_addr.sin_port = htons(portNum) ; // port number to listen to.
	receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY) ; // listen on any IP address.

	if (bind(receiver_s , (struct sockaddr *)&receiver_addr , sizeof(receiver_addr)) < 0) // binds address information to socket.
	{
		printf("bind() failed.  exiting.  \n") ;
		exit(-1) ;
	} 

	// receive messages from sender.

	printf("\n> waiting for message...\n\n") ;

	addr_len = sizeof(sender_addr) ;

	while (1)
	{
		retcode = recvfrom(receiver_s , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&sender_addr , &addr_len) ;

		if (retcode < 0)
		{
			printf("recvfrom() failed.  exiting.  \n") ;
			exit(-1) ;
		} 

		if (strcmp(receive_buffer , "end transmission") == 0) 
		{
			printf("\n> file transfer complete.\n") ;	
			break ;
		}
		else printf("\n> message received:  %s\n" , receive_buffer) ;
	}

	// finished transmission.

	if (close(receiver_s) < 0)
	{
		printf("close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	return 0 ;
}