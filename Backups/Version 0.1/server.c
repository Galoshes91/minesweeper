#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>


#define RANDOM_NUMBER_SEED 42
#define BACKLOG 10     /* how many pending connections queue will hold */
#define RETURNED_ERROR -1


char userTest[] = "admin";
char passTest[] = "root";


void readAccounts(void) {

	//read from Authentication.txt, save as array?


}




int main(int argc, char* argv[]) {

	//

	srand(RANDOM_NUMBER_SEED);
	
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	//int i=0;

	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(atoi(argv[1]));     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server starts listening ...\n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a sepetate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* this is the child process */

			/* Call method to recieve array data */
			//int *results = Receive_Array_Int_Data(new_fd,  ARRAY_SIZE);	

			/* Print out the array results sent by client */
			/*for (i=0; i < ARRAY_SIZE; i++) {
				printf("Value of index[%d] = %d\n", i, results[i]);
			}			

			free(results);*/
			
			char stringR[100];
			char stringR2[100];
			//number_of_bytes=
			recv(new_fd, &stringR, 100, 0);
			recv(new_fd, &stringR2, 100, 0);
			printf("%s", stringR);
			printf("%s", stringR2);
		

			if (send(new_fd, "All of array data received by server\n", 40 , 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
	}
	
	
	
	
	
/*	SRAND TESTING CODE
	printf("%d\n", rand()%100);
	printf("%d\n", rand()%100);
	printf("%d\n", rand()%100);
*/


	return 0;


	//

}
