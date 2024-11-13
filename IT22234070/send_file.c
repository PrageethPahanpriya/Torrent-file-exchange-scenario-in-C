#include "sock.h"

//Function to send a file over a socket to a client
int send_file(FILE *fp, int sockfd){

	char data[BUFFER_SIZE];
	long file_size;
	bzero(data,BUFFER_SIZE);

	//get the size of the file by seeking to the end of the file, ftell() to get the position (size)
	fseek(fp, 0L, SEEK_END); //move the file pointer to the end of the file
	file_size = ftell(fp); //get the file size (current position in the file is the size)
	rewind(fp); //move the file pointer back to the beginning of the file for reading

	//send file size to client
	if(send(sockfd, &file_size, sizeof(file_size), 0) == -1){

		perror("Failed to send file size.");
		exit(1); //Exit the program on failure
	}
	//read data from the file and send it to the client in chunks
	while(fread(data, sizeof(char), BUFFER_SIZE, fp) > 0){

		if(send(sockfd, data, BUFFER_SIZE, 0) == -1){

			perror("Failed to send file");
			exit(1);

		}
		bzero(data, BUFFER_SIZE); //Clear the buffer after sending the data to avoid sending old data

	}
	printf("\nFile send successfully.\n");

	shutdown(sockfd, SHUT_WR); //close write-side of socket to signal end of transfer.

	return 0; //return 0 if success

}
