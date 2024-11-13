#include "sock.h"
#include <sys/wait.h>

//Signal handler function for SIGCHLD,handles child process termination
void sig_child(int signo){

	pid_t pid; //Variable to store the process ID of the terminated child
	int stat; //Variable to store the exit status of the child process

	//printf("SIGCHILD received\n");

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		//printf("child %d terminated\n",pid);
		return;

}
