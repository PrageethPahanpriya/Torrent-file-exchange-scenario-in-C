#include        "sock.h"


int main(int argc, char **argv){

        int listenfd; //listening socket file descriptor
	int connfd[MAX_CLIENTS]; //Array to hold client connection file descriptors
	int i=0,trans_status;
	pid_t	childpid; //PID for forked child process
	socklen_t clilen; //Length of client address structure
	const char *ptr; //Pointer for client address conversion
	void sig_child(int); //Handling child termination
	char buffer[BUFFER_SIZE]; //Buffer for reading data
	char file_name[BUFFER_SIZE]; //Buffer for storing the file name
	FILE *fp; //pointer for the file to be sent
	FILE *log_file; //pointer for the log file
	char *direc = "/home/prageeth/Desktop/IT22234070/shared_files";
	
	

	//creating socket address stucture
        struct sockaddr_in servsock, clisock;

	//open log file for appending;
	log_file = fopen("log_srv4070", "a");
    	if (log_file == NULL) {
        	perror("Failed to open log file");
        	exit(1);
    	}
	
	//creating socket
        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
                perror("Socket creation failed\n");
                exit(1);
        }

        bzero(&servsock,sizeof(servsock));
	

	//assigneing port and ip addr
        servsock.sin_family = AF_INET;
        servsock.sin_port = htons(2223);
        servsock.sin_addr.s_addr = inet_addr(argv[1]);

	//bind the socket
        if ((bind(listenfd, (struct sockaddr*)&servsock, sizeof(servsock))) < 0){
                perror("bind failed\n");
                exit(1);
        }

	printf("Waiting for client connections.....\n");
	
	//listen incoming connection
        if(listen(listenfd,100) < 0){
                perror("listen failed\n");
                exit(1);
        }

	//handle zombie process
	signal(SIGCHLD,sig_child);
	

        
	//Main loop to accept multiple clients
        for(i=0; i < MAX_CLIENTS; i++){		
		bzero(&clisock,sizeof(clisock));
		clilen = sizeof(clisock); //size of the client address structure

		//accept the client connection
	        if( (connfd[i] = accept(listenfd, (struct sockaddr*)&clisock , &clilen)) < 0 ){
			if(errno == EINTR){
				continue;
			}
			else{
				perror("accept error\n");
				exit(1);
			}
		}


		//getting clients details
		if( (ptr = inet_ntop(AF_INET, &clisock.sin_addr, buffer, sizeof(buffer))) == 0 ) {
			perror("inet_ntop error \n");
			exit (1);
		}
		printf("Connection from IP %s, port %d\n", ptr, ntohs(clisock.sin_port));
		ptr = NULL; //Clear pointer after use


		//creating child process server
		if((childpid =  fork())==0)
		{
			close(listenfd);//close child listen
			
			send_list(connfd[i]); //send a list of files to the client
			
			//initialization for file handling
			int file_number, file_count = 1;
			struct dirent *de; //Structure to hold directory entries
			DIR *dr = opendir(direc); //Open the directory

			if (dr == NULL) {
    				perror("Could not open directory");
    				exit(1);
			}

			bzero(buffer,BUFFER_SIZE);
			
			//get file number from client
			read(connfd[i], buffer, sizeof(buffer));
			file_number = atoi(buffer); //Convert string to integer

			//read each enty of directory
			while ((de = readdir(dr)) != NULL) {
				if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) { //ignore . and  ..
					if (file_count == file_number){ 
						
						char full_path[BUFFER_SIZE];

						//get file name from de and append it with folder path
            					snprintf(full_path, sizeof(full_path), "%s/%s", direc, de->d_name); 

						//open file for reading
            					fp = fopen(full_path, "rb");//read binary
						if (fp == NULL) {//can access data 1 by 1(fp)
							perror("Error opening file");
							exit(1);
	    					}

						//printf("File opened successfully.\n");
						
						//send file name to client
						strcpy(file_name,de->d_name);
						send(connfd[i], file_name, sizeof(file_name), 0);
						
					}

					file_count++;

				}
			}

			//sending file to the client
			trans_status = send_file(fp,connfd[i]);

			log_info(log_file, &clisock, file_name, trans_status);

			fclose(fp); //close the file client connection
			close(connfd[i]);
			exit(0); //exit child process					
		
		}
		
		close(connfd[i]); //Close the client connection in the parent process

        }

      

        return 0;

}
