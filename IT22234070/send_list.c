#include "sock.h"

//Function to send a list of files in a directory to the client over the socket
void send_list(int sockfd){

	struct dirent *de; //structure to hold directory entries mp3 or file type
	DIR *dr = opendir("/home/prageeth/Desktop/IT22234070/shared_files");
	char file_list[BUFFER_SIZE];
	int file_number = 1;
	char file_entry[BUFFER_SIZE];

	bzero(file_list, BUFFER_SIZE);

	if(dr == NULL){
		printf("Could not open directory.");
		exit(1);
	}
	//Loop through each entry in the directory
	while ((de = readdir(dr)) != NULL){
		
		//Ignore special entries "." and ".."
		if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){

			bzero(file_entry, BUFFER_SIZE);

			//string that represents the file number and file name
			snprintf(file_entry, sizeof(file_entry), "%d. %s\n", file_number, de->d_name);

			//Append the current file entry to the main file list buffer
			strcat(file_list, file_entry);
			file_number++;
		}
	}
	closedir(dr); //Close the directory after processing all files

	//Send the complete file list to the client
	if(send(sockfd, file_list, strlen(file_list), 0) == -1){
		printf("Failed to send files.");
		exit(1);
	}
}
