// reliable udp sender
// created 19 november 2017
// jim bui and luis franco

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h> // socket stuff.
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

int main()
{
	char fileName[] = "fileSend.dat" ;
	char destIpAddr[] = "131.247.3.46" ; // send to this IP address.
	int destPortNum = 6008 ; // send to this port.
	int options = 0 ;

	int retcode = sendFile(&fileName , &destIpAddr , destPortNum , options) ; 

	if (retcode == 0) printf("\n> File sent successfully.\n\n") ;
	else printf("\n> File failed to send successfully.\n\n") ;

	return retcode ;
}

int sendFile(char *fileName , char *destIpAddr , int destPortNum , int options)
{
	int sender_s ; // this is the socket that we are sending from.
	struct sockaddr_in server_addr ; // this is the destination address informationne.
	int addr_len ;
	char send_buffer[4096] ;
	// char receive_buffer[4096] ;
	int retcode ;
	int file ; // file to send.

	// create socket to send.

	sender_s = socket(AF_INET , SOCK_DGRAM , 0) ;

	if (sender_s < 0)
	{
		printf("socket() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// destination address specifics.

	server_addr.sin_family = AF_INET ; // address family to use.
	server_addr.sin_port = htons(destPortNum) ; // port number to send to.
	server_addr.sin_addr.s_addr = inet_addr(destIpAddr) ; // IP address to send to.

	// create messages and send.  
	// #TODO: implement creating packet from file and sliding window protocol.
/*
	fh = open(fileName, O_RDONLY, S_IREAD | S_IWRITE);
	if (fh == -1)
	{
		printf("() failed.  exiting.  \n" , sendFile) ;
		exit(1);
	  }
*/
	char message[200] ;
	int i = 0 ;

	while (i < 200)
	{	
		sprintf(message , "%d" , i) ;

		strcpy(send_buffer , message) ;
		retcode = sendto(sender_s , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&server_addr , sizeof(server_addr)) ;
		i++ ;
		// usleep(1000000) ; // this is one second.
		usleep(10000) ;
	}

	strcpy(send_buffer , "end transmission") ;
	retcode = sendto(sender_s , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&server_addr , sizeof(server_addr)) ;

	if (retcode < 0)
	{
		printf("sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// finished transmitting.

	retcode = close(sender_s) ;

	if (retcode < 0)
	{
		printf("close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// printf("%s" , fileName) ;
	return 0 ; // return 0 for success , -1 for failure.
} 