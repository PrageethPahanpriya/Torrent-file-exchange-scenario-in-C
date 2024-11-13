#include "sock.h"

int main(int argc, char **argv){

        int     srvfd, a, selected_file_number; //Socket file descriptor, connection status, file number chosen by the user
        char    buffer[BUFFER_SIZE]; //buffer for reading data from the server
	char    send_buffer[BUFFER_SIZE]; //Buffer for sending data to the server

        struct sockaddr_in      servsock; //Structure to hold server socket information

        if ((srvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
                printf("Socket creation failed\n");
                exit(1);
        }


        bzero(&servsock, sizeof(servsock));

        //here socket addres structure shold contain servers address and its port number
        servsock.sin_family = AF_INET;
        servsock.sin_port   = htons(2223);
        servsock.sin_addr.s_addr = inet_addr(argv[1]);

        //Connect to the server; 'a' checks if the connect function was successful
        a = connect(srvfd, (struct sockaddr*)&servsock,  sizeof(servsock));

        if(a == 0){
                printf("connection successfull\n");
        }
        else{
                printf("connect error");
                exit(1);
        }


	bzero(buffer,BUFFER_SIZE);

	//Read the list of available in files from the server and print them(send_list)
        read(srvfd, buffer, BUFFER_SIZE);
        printf("Available files:\n%s\n", buffer);


	printf("Enter the file number to download : ");
	scanf("%d", &selected_file_number);

	//Convert the selected file number to a string and prepare to send it to the server
	snprintf(send_buffer, sizeof(send_buffer), "%d", selected_file_number);
        write(srvfd, send_buffer, strlen(send_buffer)); 		//Send the selected file number to the server

	//Clear the buffer again to receive file data
	bzero(buffer,BUFFER_SIZE);
	read(srvfd, buffer, sizeof(buffer)); //Read data from the server

	//Call a function to receive the file from the server
	recv_file(srvfd,buffer);


}
