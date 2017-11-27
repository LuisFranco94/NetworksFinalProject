// reliable udp sender
// created 19 november 2017
// jim bui and luis franco

// example run:  ./reliable_udp_sender 131.247.3.46 6028 humble.dat
// compiling:  gcc reliable_udp_sender.c -o reliable_udp_sender

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc , char *argv[])
{
	char destination_ip_address[20] ; // send to this IP address.
	int destination_port_number ; // send to this port.
	char file_name[20] ;
	int options = 0 ; // unused right now.

	if (argc != 4)
	{
		printf("\n") ;
		printf("> usage:  ./a.out destination_ip_addr destination_port_number file_name\n") ;
		printf("> exiting.\n\n") ;
		return -1 ;
	}

	strcpy(destination_ip_address , argv[1]) ;
	destination_port_number = atoi(argv[2]) ;
	strcpy(file_name , argv[3]) ; 

	// example run:  ./reliable_udp_sender 131.247.3.46 6028 humble.dat

	int return_code = sendFile(&file_name , &destination_ip_address , destination_port_number , options) ; // main function call.
	if (return_code == 0) printf("> File sent successfully.\n\n") ;
	else printf("> File failed to send successfully.\n\n") ;

	return return_code ;
}

int sendFile(char *file_name , char *destination_ip_address , int destination_port_number , int options)
{
	int sender_socket ; // this is the socket that we are sending from.
	struct sockaddr_in receiver_address ; // this is the destination address information.
	int address_length ;
	int return_code ;

	int file_handle ;
	int file_offset = 0 ; // where the pointer is for pread().  decrement to go back.
	int file_read_size = 4085 ; // how many characters to set.
	char file_read_array[file_read_size] ; // contains read data.
	char send_buffer[4096] ; // final array to send.  consists of packet_header and file_read_array.
	char receive_buffer[4096] ; // data received.  currently not in use.
	int data_input_length ; // how many characters were read.

	char packet_header[11] ; // [UDP][FIN - 1 bit][SEQUENCE NUMBER - 10 bits][DATA - variable bits][UDP]
	int sequence_number = 1000000000 ; // we will not randomize because security is not an issue.
	int fin_val = 0 ;

	char received_sequence_char[10] ;
	int received_sequence_int ;
	int expected_sequence_int ;
	char received_flags[1] ;

	int packet_lost = 0 ;
	int packet_good = 0 ;
	int resend = 0 ;

	// create socket to send.

	sender_socket = socket(AF_INET , SOCK_DGRAM , 0) ;

	if (sender_socket < 0)
	{
		printf("socket() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// destination address specifics.

	receiver_address.sin_family = AF_INET ; // address family to use.
	receiver_address.sin_addr.s_addr = inet_addr(destination_ip_address) ; // ip address to send to.
	receiver_address.sin_port = htons(destination_port_number) ; // port number to send to.

	// set time-out for recv.

	struct timeval tv ;
	tv.tv_sec = 0 ; // 10 seconds time-out.
	tv.tv_usec = 100000 ;
	setsockopt(sender_socket , SOL_SOCKET , SO_RCVTIMEO , (const char*)&tv , sizeof(struct timeval)) ;

	// open file.

	file_handle = open(file_name , O_RDONLY , S_IREAD | S_IWRITE) ; // open file.

	if (file_handle == -1)
	{
		printf("open() failed.  exiting.  \n" , sendFile) ;
		exit(-1) ;
	}

	// establish connection.

	/*
		send SYN and SEQUENCE_NUMBER.
		receive (blocking) SYN , ACK , and SEQUENCE_NUMBER.
		send ACK and SEQUENCE_NUMBER + 1.
	*/

	// set syn to 1 and send packet.

	fin_val = 0 ;

	snprintf(packet_header , 12 , "%i%i" , fin_val , sequence_number) ;
	expected_sequence_int = sequence_number + 1 ;
	sequence_number++ ;
	strcpy(send_buffer , packet_header) ;
	printf("> initial send:  %s\n" , send_buffer) ;
	return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;

	if (return_code < 0)
	{
		printf("sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// receieve syn-ack.

	memset(receive_buffer , '\0' , 4096) ; // clear buffer.
	return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // blocking

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

	// printf("> receive_buffer: %s\n" , receive_buffer) ;

	// sscanf(received_sequence_char , "%d" , &received_sequence_int) ;
/*
	if (receive_buffer[0] == '\0' || (receive_buffer[0] != 1 && receive_buffer[1] != 1 && received_sequence_int != expected_sequence_int))
	{
		printf("\n> connection failed.\n") ;
		printf("> exiting.\n\n") ;
		exit(-1) ;
	}
*/
	// printf("\n> received_sequence_int: %i\n" , received_sequence_int) ;

	// retcode = recvfrom(client_s , in_buf , sizeof(in_buf) , 0 , (struct sockaddr *)&server_addr , &addr_len) ;
	// printf("\n> received:  %s" , receive_buffer) ;
	// if the packet received is not SYN-ACK and SEQUENCE_NUMBER + 1 , quit.

	// create messages and send.

	do // main protocol function.
	{	
		memset(send_buffer , '\0' , 4096) ; // clear buffer.
		memset(file_read_array , '\0' , file_read_size) ;
		data_input_length = pread(file_handle , file_read_array , file_read_size , file_offset) ; // pread(file to read from , put into this , read up to this many bytes , offset)

		if (data_input_length > 0)
		{
			// create packet header.

			fin_val = 0 ;
			snprintf(packet_header , 12 , "%i%i" , fin_val , sequence_number) ;
			expected_sequence_int = sequence_number ;
			sequence_number++ ;

			// assemble message into send_buffer.

			strcpy(send_buffer , packet_header) ;
			strcat(send_buffer , file_read_array) ;

			// send send_buffer.
			printf("> sending:  %s\n" , packet_header) ;
			// printf("\n> sending:  %s" , send_buffer) ;
			return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;
			file_offset = file_offset + file_read_size ;

			if (return_code < 0)
			{
				printf("sendto() failed.  exiting.  \n") ;
				exit(-1) ;
			}

			// wait to receive reply.

  			address_length = sizeof(receiver_address) ;
  			memset(receive_buffer , '\0' , sizeof(receive_buffer)) ; // clear buffer.
  			// return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , MSG_DONTWAIT , (struct sockaddr *)&receiver_address , &address_length) ; // non-blocking
  			return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // blocking.
  			// printf("\n> received buffer:  %s" , receive_buffer) ;

			if (return_code < 0)
			{
				printf("recvfrom() failed.  resending.\n") ;
				resend++ ;
				sequence_number = expected_sequence_int ;
				file_offset = file_offset - (expected_sequence_int - received_sequence_int) * file_read_size ;

				if (resend == 15)
				{
					printf("> connection lost.  exiting.  \n") ;
					exit(-1) ;
				}
				// exit(-1) ;
			}

			else
			{
				
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
				
				printf("> received int:  %i\n\n" , received_sequence_int) ;
				// printf("\n> received char:  %i" , received_sequence_char) ;
				// printf("\n> received buffer:  %i" , receive_buffer) ;

				if (received_sequence_int != expected_sequence_int)
				{
					packet_lost++ ;
					printf("> expected:  %i\n" , expected_sequence_int) ;
					printf("> lost a packet!  total lost: %i total good: %i\n" , packet_lost , packet_good) ;
					printf("> difference: %i\n\n" , expected_sequence_int - received_sequence_int) ;
					file_offset = file_offset - (expected_sequence_int - received_sequence_int) * file_read_size ;
					sequence_number = expected_sequence_int ;
					recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // receives any bad packets.
					// sleep(1) ;
					// exit(-1) ;
				}

				else
				{
					packet_good++ ;
					resend = 0 ;
				}
			}

			// printf("\n> received:  %s" , receive_buffer) ;
		}
	} while (data_input_length > 0) ;

	// end transmission.

	fin_val = 1 ;

	snprintf(packet_header , 12 , "%i%i" , fin_val , sequence_number) ;
	// snprintf(packet_header , 14 , "%i%i%i%i" , syn_val , ack_val , fin_val , sequence_number) ; // packet won't send without the x , idk why.
	strcpy(send_buffer , packet_header) ;
	// printf("\n> send the end:  %s" , send_buffer) ;
	return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;

	if (return_code < 0)
	{
		printf("sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// close socket.

	return_code = close(sender_socket) ;

	if (return_code < 0)
	{
		printf("close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	return 0 ; // return 0 for success , -1 for failure.
} 