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

int main(int argc , char *argv[])
{
	char receivedFile[] = "received_file.dat" ;
	int portNum = 6028 ;
	int maxSize = 10 ;
	int options = 0 ;

	if (argc != 3)
	{
		printf("\n") ;
		printf("> usage:  ./a.out port_number file_output\n") ;
		printf("> example:  ./a.out 6008 received_file.dat\n") ;
		printf("> exiting.\n\n") ;
		return -1 ;
	}

	portNum = atoi(argv[1]) ;
	strcpy(receivedFile , argv[2]) ;

	int retcode = recvFile(&receivedFile , portNum , maxSize , options) ; 
}

int recvFile(char *receivedFile , int portNum , int maxSize , int options)
{
	int receiver_s ;
	struct sockaddr_in receiver_addr ;
	struct sockaddr_in sender_addr ;
	int addr_len ;

	char receive_buffer[4096] ;
	char message_buffer[4096] ;
	char output_buffer[4096] ;

	int retcode ;
	struct in_addr client_ip_addr ; 

	int sequence_number = 1000000000 ;
	int next_sequence_number = -1 ; // hold which packet we need next.

	char received_sequence_char[12] ; // data manipulation for received messages.
	int received_sequence_int ;
	int expected_sequence_int ;
	char received_flags[3] ;

	int packet_bad = 0 ;
	int packet_good = 0 ;

	// create socket.

	receiver_s = socket(AF_INET , SOCK_DGRAM , 0) ;

	if (receiver_s < 0)
	{
		printf("> socket() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// fill socket data.  confirm if this is destination information.

	receiver_addr.sin_family = AF_INET ;
	receiver_addr.sin_port = htons(portNum) ;
	receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY) ; 

	if (bind(receiver_s , (struct sockaddr *)&receiver_addr , sizeof(receiver_addr)) < 0) // binds address information to socket.
	{
		printf("> bind() failed.  exiting.  \n") ;
		exit(-1) ;
	} 

	addr_len = sizeof(sender_addr) ;

	// open file to write.

	FILE *file ;
	file = fopen(receivedFile , "w+") ;

	// receive messages from sender.

	printf("\n") ;
	printf("> waiting for message...\n") ;
	
	while (1) // receive message.
	{
		memset(receive_buffer , '\0' , sizeof(receive_buffer)) ;
		retcode = recvfrom(receiver_s , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&sender_addr , &addr_len) ;

		if (retcode < 0)
		{
			printf("> recvfrom() failed.  exiting.  \n") ;
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

		if (received_flags[0] == '1') // signifies end of transmission.  reply with ACK and exit loop.
		{
			printf("> received_sequence_int: %i\n" , received_sequence_int) ;
			memset(output_buffer , '\0' , sizeof(receive_buffer)) ; 
			snprintf(output_buffer , 12 , "%i%i" , 0 , received_sequence_int) ;
			printf("> sending: %s\n" , output_buffer) ;
			retcode = sendto(receiver_s , output_buffer , (strlen(output_buffer) + 1) , 0 , (struct sockaddr *)&sender_addr , sizeof(sender_addr)) ;

			if (retcode < 0)
			{
				printf("sendto() failed.  exiting.  \n") ;
				exit(-1) ;
			}

			printf("> packet_good: %i packet_bad: %i\n" , packet_good , packet_bad) ;
			printf("> file transfer complete.\n\n") ;
			fclose(file) ;

			break ;
		}

		else
		{
			printf("> received_sequence_int: %i\n" , received_sequence_int) ;

			if (next_sequence_number == -1) // signifies first sequence received.  initialize values.
			{
				next_sequence_number = received_sequence_int ;
				next_sequence_number++ ;
				memset(message_buffer , '\0' , sizeof(message_buffer)) ; // clear buffer.
				memcpy(message_buffer , &receive_buffer[11] , sizeof(message_buffer)) ;
			}

			else if (received_sequence_int == next_sequence_number) // correct message is received , write to file.
			{
				next_sequence_number++ ;
				memset(message_buffer , '\0' , sizeof(message_buffer)) ;
				memcpy(message_buffer , &receive_buffer[11] , sizeof(message_buffer)) ;

				fputs(message_buffer , file) ;
				packet_good++ ;
			}

			else // otherwise do nothing.
			{
				printf("> bad packet!  discarded!\n") ;
				packet_bad++ ;
			}
		}

		memcpy(&client_ip_addr , &sender_addr.sin_addr.s_addr , 4) ;

		if (received_flags[0] == '2') // signifies ACK reply requested.  send reply with needed packet number.
		{
			memset(output_buffer , '\0' , sizeof(receive_buffer)) ;

			if (received_sequence_int == next_sequence_number - 1)
			{
				snprintf(output_buffer , 12 , "%i%i" , 0 , next_sequence_number - 1) ;
			}

			else
			{
				snprintf(output_buffer , 12 , "%i%i" , 0 , next_sequence_number) ;
			}

			printf("> sending: %s\n" , output_buffer) ;
			retcode = sendto(receiver_s , output_buffer , (strlen(output_buffer) + 1) , 0 , (struct sockaddr *)&sender_addr , sizeof(sender_addr)) ;

			if (retcode < 0)
			{
				printf("> sendto() failed.  exiting.  \n") ;
				exit(-1) ;
			}
		}
	}

	if (close(receiver_s) < 0)
	{
		printf("> close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	return 0 ;
}