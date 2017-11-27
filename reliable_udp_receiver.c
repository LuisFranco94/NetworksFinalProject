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
	int portNum = 6028 ; // 6008 and 6028 should work.
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
	char message_buffer[4096] ;
	char output_buffer[4096] ;
	int retcode ;
	struct in_addr client_ip_addr ; 

	int sequence_number = 1000000000 ;
	int acknowledgement_number ;

	char received_sequence_char[12] ;
	int received_sequence_int ;
	int expected_sequence_int ;
	char received_flags[3] ;

	int next_sequence_number = -1 ;

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

	printf("\n> waiting for message...") ;

	addr_len = sizeof(sender_addr) ;

	// open file.

	FILE *file ;
	file = fopen("received_file.dat" , "w+") ;
	// fprintf(file, "%s %s %s %d", "We", "are", "in", 2012) ;
	// fclose(file) ;

	// establish connection.

	/*
		receive SYN and SEQUENCE_NUMBER.
		send SYN , ACK , and SEQUENCE_NUMBER.
		receive ACK and SEQUENCE_NUMBER + 1.
	*/

	/*

	memset(receive_buffer , '\0' , sizeof(receive_buffer)) ; // clear buffer.
	retcode = recvfrom(receiver_s , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&sender_addr , &addr_len) ; // blocking , wait for SYN and SEQUENCE_NUMBER.
	
	received_flags[0] = receive_buffer[0] ;
	received_flags[1] = receive_buffer[1] ;
	received_flags[2] = receive_buffer[2] ;

	received_sequence_char[0] = receive_buffer[3] ;
	received_sequence_char[1] = receive_buffer[4] ;
	received_sequence_char[2] = receive_buffer[5] ;
	received_sequence_char[3] = receive_buffer[6] ;
	received_sequence_char[4] = receive_buffer[7] ;
	received_sequence_char[5] = receive_buffer[8] ;
	received_sequence_char[6] = receive_buffer[9] ;
	received_sequence_char[7] = receive_buffer[10] ;
	received_sequence_char[8] = receive_buffer[11] ;
	received_sequence_char[9] = receive_buffer[12] ;
	
	sscanf(received_sequence_char , "%d" , &received_sequence_int) ;

	if (received_flags[0] != '1') // receieve SYN.
	{
		printf("\n> connection failed.\n") ;
		printf("> exiting.\n\n") ;
		exit(-1) ; 
	}

	receive_buffer[12] = '1' ; 
	strcpy(output_buffer , receive_buffer) ;
	retcode = sendto(receiver_s , output_buffer , (strlen(output_buffer) + 1) , 0 , (struct sockaddr *)&sender_addr , sizeof(sender_addr)) ;

	if (retcode < 0)
	{
		printf("sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	printf("\n") ;

	*/

	while (1) // receive message.
	{
		memset(receive_buffer , '\0' , sizeof(receive_buffer)) ; // clear buffer.
		retcode = recvfrom(receiver_s , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&sender_addr , &addr_len) ;

		// sscanf(received_sequence_char , "%d" , &received_sequence_int) ;

		if (retcode < 0)
		{
			printf("recvfrom() failed.  exiting.  \n") ;
			exit(-1) ;
		} 

		received_flags[0] = receive_buffer[0] ;

		received_sequence_char[0] = receive_buffer[1] ;
		received_sequence_char[1] = receive_buffer[2] ;
		received_sequence_char[2] = receive_buffer[3] ;
		received_sequence_char[3] = receive_buffer[4] ;
		received_sequence_char[4] = receive_buffer[5] ;
		received_sequence_char[5] = receive_buffer[6] ;
		received_sequence_char[6] = receive_buffer[7] ;
		received_sequence_char[7] = receive_buffer[8] ;
		received_sequence_char[8] = receive_buffer[9] ;
		received_sequence_char[9] = receive_buffer[10] ;

		sscanf(received_sequence_char , "%i" , &received_sequence_int) ;

		if (received_flags[0] == '1') 
		{
			printf("\n> file transfer complete.\n") ;
			fclose(file) ;
			break ;
		}

		else 
		{
			// printf("\n> received_flags: %c\n" , received_flags[2]) ;
			printf("\n> received_sequence_int: %i\n" , received_sequence_int) ;

			if (next_sequence_number == -1)
			{
				next_sequence_number = received_sequence_int ;

				next_sequence_number++ ;
				memset(message_buffer , '\0' , sizeof(message_buffer)) ; // clear buffer.
				memcpy(message_buffer , &receive_buffer[11] , sizeof(message_buffer)) ;
				// printf("writing message to file:  %s\n" , message_buffer) ; // i am assuming this is correct.

				fputs(message_buffer , file) ; // write message to file.
			}

			else if (received_sequence_int == next_sequence_number) // correct message is received.
			{
				next_sequence_number++ ;
				memset(message_buffer , '\0' , sizeof(message_buffer)) ; // clear buffer.
				memcpy(message_buffer , &receive_buffer[11] , sizeof(message_buffer)) ;
				// printf("writing message to file:  %s\n" , message_buffer) ; // i am assuming this is correct.

				fputs(message_buffer , file) ; // write message to file.
			}
			// printf("\n> header message: %c" , receive_buffer[12]) ;
			// printf("\n> message received:  \n%s\n" , receive_buffer) ; // print whole message.
		}

		memcpy(&client_ip_addr , &sender_addr.sin_addr.s_addr , 4) ;

		memset(output_buffer , '\0' , sizeof(receive_buffer)) ; // clear buffer.
		snprintf(output_buffer , 12 , "%i%i" , 0 , received_sequence_int) ;
		// strcpy(output_buffer , "FUCK") ;
		printf("\n> sending: %s\n" , output_buffer) ;
		retcode = sendto(receiver_s , output_buffer , (strlen(output_buffer) + 1) , 0 , (struct sockaddr *)&sender_addr , sizeof(sender_addr)) ;

		if (retcode < 0)
		{
			printf("sendto() failed.  exiting.  \n") ;
			exit(-1) ;
		}
	}

	// finished transmission.

	if (close(receiver_s) < 0)
	{
		printf("close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	return 0 ;
}