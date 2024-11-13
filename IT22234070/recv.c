#include "sock.h"

//function to receive a file from the server over the socket
void recv_file(int sockfd, char *filename){

	int bytes_received;
	int last_printed_progress = 0;
	FILE *fp; //file pointer to store the received file
	char *direc = "/home/prageeth/Desktop/IT22234070/download_path";
	char full_path[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	long file_size;
	long total_bytes_received = 0;
	float progress = 0;

	bzero(buffer, BUFFER_SIZE);
	bzero(full_path, BUFFER_SIZE);

	//Create the full file path by appending the directory and the file name
	snprintf(full_path, sizeof(full_path), "%s/%s", direc, filename);

	//Open the file in write-binary mode to store the incoming data
	fp = fopen(full_path, "wb");

	if (fp == NULL) {
    		perror("Failed to open file\n");
    		exit(1);
	}

	// Receive the file size from the server
    	if (recv(sockfd, &file_size, sizeof(file_size), 0) <= 0) {
        	perror("Failed to receive file size");
        	exit(1);
    	}

	printf("Downloading file of size: %ld bytes\n", file_size);

	//main loop to receive the file data in chunks
	while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {

		//Write the received data into the file
    		fwrite(buffer, sizeof(char), bytes_received, fp);
		total_bytes_received += bytes_received; //Update the total number of bytes received

		// Calculate and display the download progress
        	progress = ((float)total_bytes_received / file_size) * 100;

		if ((int)progress >= last_printed_progress + 10 ) {
        		last_printed_progress = (int)progress;
        		printf("Downloading progress: %d%%\n", last_printed_progress); 	

        		//printf("Download progress: %.2f%%", progress); 
        		fflush(stdout); 
		}
    		bzero(buffer, BUFFER_SIZE);
	}
	
	if (bytes_received == -1) {
    		perror("Error receiving file\n");
    		exit(1);
	}else{
		printf("\nDownload complete.\n");
	}

	//lose the file after the transfer is complete
	fclose(fp);

}
