// reliable udp sender
// created 19 november 2017
// jim bui and luis franco

// example run:  ./reliable_udp_sender 131.247.3.46 6028 humble.dat 4
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
	char destination_ip_address[20] ;
	int destination_port_number ;
	char file_name[20] ;
	int backoff = 0 ; // backoff value.
	int enable_loss ;

	if (argc != 6)
	{
		printf("\n") ;
		printf("> usage:  ./a.out destination_ip_addr destination_port_number file_name back_off_value enable_loss\n") ;
		printf("> example:  ./a.out 131.247.3.46 6028 100MB_file.dat 4 0\n") ;
		printf("> exiting.\n\n") ;
		return -1 ;
	}

	strcpy(destination_ip_address , argv[1]) ;
	destination_port_number = atoi(argv[2]) ;
	strcpy(file_name , argv[3]) ; 
	backoff = atoi(argv[4]) ;
	enable_loss = atoi(argv[5]) ;

	int return_code = sendFile(&file_name , &destination_ip_address , destination_port_number , backoff , enable_loss) ;

	if (return_code == 0) 
	{
		printf("> file sent successfully.\n") ;
	}

	else 
	{
		printf("> file failed to send successfully.\n") ;
	}

	return return_code ;
}

int sendFile(char *file_name , char *destination_ip_address , int destination_port_number , int backoff , int enable_loss)
{
	int sender_socket ; // this is the socket that we are sending from.
	struct sockaddr_in receiver_address ; // this is the destination address information.
	int address_length ;
	int return_code ;

	int file_handle ;
	int file_offset = 0 ; // where the pointer is for pread().  decrement to go back.
	int file_read_size = 4085 ; // how many characters to try to read from file.
	int data_input_length ; // how many characters were actually read from file.
	char file_read_array[file_read_size] ; // contains the read data.
	char send_buffer[4096] ; // buffer to send.  consists of packet_header and file_read_array.
	char receive_buffer[4096] ; // data received (sequence numbers as ACKs are received).

	char packet_header[11] ; // [UDP][FLAG_VAL][SEQUENCE_NUMBER][DATA][UDP]
	int sequence_number = 1000000000 ; // 10 bits exactly.
	int flag_val = 0 ; // one bit exactly.  [0 - no flag][1 - finish transmission][2 - request ACK]

	char received_sequence_char[10] ; // data manipulation for received messages.
	int received_sequence_int ;
	int expected_sequence_int ;
	char received_flags[1] ;

	int packet_bad = 0 ; // counters for optimization.
	int packet_good = 0 ;

	int resend = 0 ; // counter to stop resending and declare lost connection.

	int window_size = 1 ; // how many messages can be sent before requesting ACK.
	int window_count = 0 ; // counts how many messages have currently been sent for this window.

	int random_value ; // for randomizing packet loss.
	int loss_percentage = 2 ; // 2 percent
	srand(time(NULL)) ;

	int data_read_end = -1 ;

	// create socket to send.

	sender_socket = socket(AF_INET , SOCK_DGRAM , 0) ;

	if (sender_socket < 0)
	{
		printf("> socket() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	// destination address specifics.

	receiver_address.sin_family = AF_INET ; // address family to use.
	receiver_address.sin_addr.s_addr = inet_addr(destination_ip_address) ; // ip address to send to.
	receiver_address.sin_port = htons(destination_port_number) ; // port number to send to.

	// set time-out for recv.

	struct timeval tv ;
	tv.tv_sec = 0 ; // seconds.
	// tv.tv_usec = 100000 ; // milliseconds.
	tv.tv_usec = 500000 ; // milliseconds.
	setsockopt(sender_socket , SOL_SOCKET , SO_RCVTIMEO , (const char*)&tv , sizeof(struct timeval)) ;

	// open file.

	file_handle = open(file_name , O_RDONLY , S_IREAD | S_IWRITE) ;

	if (file_handle == -1)
	{
		printf("> open() failed.  exiting.  \n" , sendFile) ;
		exit(-1) ;
	}

	// establish connection and confirm message was received.

	flag_val = 2 ; // requesting ACK for initial send.

	snprintf(packet_header , 12 , "%i%i" , flag_val , sequence_number) ;
	expected_sequence_int = sequence_number + 1 ;
	sequence_number++ ;
	strcpy(send_buffer , packet_header) ;
	printf("> initial send:  %s\n" , send_buffer) ;
	return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;

	if (return_code < 0)
	{
		printf("> sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	memset(receive_buffer , '\0' , 4096) ;
	return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // blocking.

	if (return_code < 0)
	{
	 	printf("> connection could not be made.  exiting.  \n") ;
		exit(-1) ;
	}

	// start data transmission.

	do
	{	 
		memset(send_buffer , '\0' , 4096) ;
		memset(file_read_array , '\0' , file_read_size) ;
		data_input_length = pread(file_handle , file_read_array , file_read_size , file_offset) ;

		if (data_input_length > 0) // if data has been read , send.  else , exit.
		{
			window_count++ ;

			// create packet header.
			
			if (window_count == window_size)
			{
				flag_val = 2 ; // set to request ACK when enough messages have been sent.
			}

			else
			{
				flag_val = 0 ; // otherwise , do not set a flag.
			}

			snprintf(packet_header , 12 , "%i%i" , flag_val , sequence_number) ;
			expected_sequence_int = sequence_number ;
			sequence_number++ ;

			// assemble message into send_buffer.

			strcpy(send_buffer , packet_header) ;
			strcat(send_buffer , file_read_array) ;

			// send message.

			printf("> sending:  %s\n" , packet_header) ;

			if (enable_loss == 1) // if packet loss is enabled.
			{
				random_value = rand() % 100 + 1 ; // pick a number between 1 and 100.
				return_code = 1 ;

				if (random_value > loss_percentage)
				{
					return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;

					if (return_code < 0)
					{
						printf("> sendto() failed.  exiting.  \n") ;
						exit(-1) ;
					}

				}
			}

			else // if packet loss is not enabled.
			{
				return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;

				if (return_code < 0)
				{
					printf("> sendto() failed.  exiting.  \n") ;
					exit(-1) ;
				}
			}

			file_offset = file_offset + file_read_size ;

			if (window_count == window_size) // wait to receive reply if enough packets were sent and ACK was requested.
			{
	  			address_length = sizeof(receiver_address) ;
	  			memset(receive_buffer , '\0' , sizeof(receive_buffer)) ;
	  			return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // blocking.
	  		
				if (return_code < 0) // if no reply was received , continue requesting ACKs.  if no reply is received enough times , declare connection lost.
				{
					printf("> recvfrom() failed.\n") ;
					resend++ ;
					window_count-- ;

					if (resend == 15)
					{
						printf("> connection lost.  exiting.  \n") ;
						exit(-1) ;
					}
				}

				else // if reply was received , confirm that sequence number in ACK is correct.  if not , go back to that sequence number and continue sending from there.
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
					
					printf("> received int:  %i\n" , received_sequence_int) ;

					if (received_sequence_int != expected_sequence_int) // if received sequence number is incorrect , decrease window size and set to resend from correct sequence number.
					{
						window_size = (window_size / backoff) + 1 ;
						window_count = 0 ;
						packet_bad = packet_bad + (sequence_number - received_sequence_int) ;
						printf("> expected:  %i\n" , expected_sequence_int) ;
						printf("> lost a packet!  packet_good: %i packet_bad: %i\n" , packet_good , packet_bad) ;
						printf("> difference: %i\n" , expected_sequence_int - received_sequence_int) ;

						if (received_flags[0] == 1)
						{
							file_offset = file_offset - (sequence_number - received_sequence_int + 1) * file_read_size ;
						}

						else if (received_flags[0] == 0)
						{
							file_offset = file_offset - (sequence_number - received_sequence_int) * file_read_size ;
						}

						else 
						{
							printf("????????????????????????????????????????????????????????????????????\n") ;
						}

						sequence_number = received_sequence_int ;
						recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ;
					}

					else // if received sequence number is correct , increase window size.
					{
						packet_good = packet_good + window_size + 1 ;
						window_size = window_size + 1 ;
						window_count = 0 ;
						resend = 0 ;
					}
				}
			}
		}

		else
		{
			data_read_end = 1 ;
		}
	} while (data_input_length > 0) ;

	if (data_read_end != 1)
	{
		return_code = close(sender_socket) ;

		if (return_code < 0)
		{
			printf("> close() failed.  exiting.  \n") ;
			exit(-1) ;
		}

		printf("> connection lost.  exiting.  \n") ;

		return -1 ;
	}

	// send packet to close connection.

	flag_val = 1 ; // set flag to signal end of file transmission.

	snprintf(packet_header , 12 , "%i%i" , flag_val , sequence_number) ;
	strcpy(send_buffer , packet_header) ;
	return_code = sendto(sender_socket , send_buffer , (strlen(send_buffer) + 1) , 0 , (struct sockaddr *)&receiver_address , sizeof(receiver_address)) ;
	printf("> sending:  %s\n" , packet_header) ;

	if (return_code < 0)
	{
		printf("> sendto() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	while (1) // confirm connection was closed correctly.
	{
		return_code = recvfrom(sender_socket , receive_buffer , sizeof(receive_buffer) , 0 , (struct sockaddr *)&receiver_address , &address_length) ; // blocking.

		if (return_code < 0)
		{
			printf("> recvfrom() failed.  resending.\n") ;
			resend++ ;

			if (resend == 15)
			{
				printf("> connection did not close correctly.  exiting.  \n") ;
				exit(-1) ;
			}
		}

		else 
		{
			break ;
		}
	}

	// close socket.

	return_code = close(sender_socket) ;

	if (return_code < 0)
	{
		printf("> close() failed.  exiting.  \n") ;
		exit(-1) ;
	}

	printf("> packet_good: %i packet_bad: %i\n" , packet_good , packet_bad) ;

	return 0 ;
} 