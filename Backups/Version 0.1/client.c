#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>

#define MAXDATASIZE 100 /* max number of bytes we can get at once */
#define TITLEBORDER "================================================"


int socketNumber;

//char *password;


int drawBoard(void) {


	//char menuOption[0];
	int menuOption;
	
	printf("    1 2 3 4 5 6 7 8\n--------------------\n");


	
	printf("A |\n");//
	printf("B |\n");//
	printf("C |\n");//
	printf("D |\n");//
	printf("E |\n");// Get mine details from the server
	printf("F |\n");//
	printf("G |\n");//
	printf("H |\n");//
	printf("I |\n");//
	
	do {
	
		printf("\nChoose an option:\n<1> Reveal tile\n<2> Place flag\n<3> Quit game\n\nOption (1,2,3):");
		scanf("%d", &menuOption);
		
		if((menuOption < 1) || (menuOption > 3)) printf("%sInvalid choice%s\n\n", TITLEBORDER, TITLEBORDER);


	} while((menuOption > 3) || (menuOption < 1));
	
	return menuOption;
	
}

int mainMenu(void) {

	int menuOption;


	printf("\n\nWelcome to the Minesweeper gaming system.\n\n");



	do {

		printf("Please enter a section: \n");
		printf("<1> Play Minesweeper\n<2> Show Leaderboard\n<3> Quit\n\n");

		printf("Selection option (1-3): ");

		scanf("%d", &menuOption);

		if((menuOption < 1) || (menuOption > 3)) printf("%sInvalid choice%s\n\n", TITLEBORDER, TITLEBORDER);


	} while((menuOption < 1) || (menuOption > 3));
	
	// Menu selection code, assuming selection is correct
	
	if (menuOption == 1) drawBoard();

	return menuOption;


}
/*
int authentication(void) {




	printf("You are required to log on with your registered user\
name and password.\n");

	printf("Username: ");
	scanf("%s", username);

	/*
	printf("Password: ");
	scanf("%s", password);


	//Program then sends this information to the server


	return 1;


}*/

int main(int argc, char *argv[]) {
	char username[100];
	char password[100];
	
	
	int menuChoice;
	
	
	/*
	
	Code for setting up the server/client connection
	
	*/
	
	int sockfd, numbytes;//, i=0;  
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	
	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	
	
	//Under dev
	socketNumber = sockfd;
	//
	
	
	/*
	
	Game code
	
	*/
	
	printf("%s\n \
Welcome to the online Minesweeper \
gaming system\n%s\n\n", TITLEBORDER, TITLEBORDER);



	//authentication();
	printf("You are required to log on with your registered user\
name and password.\n");

	printf("Username: ");
	scanf("%s", username);
	send(sockfd, username, sizeof(username), 0);
	
	printf("Password: ");
	scanf("%s", password);
	
	send(sockfd, username, sizeof(password), 0);

	menuChoice = mainMenu();
	
	
	
	//printf("%s", username);

	
	//drawBoard();



	//int test = authenticate();
	//printf("%i", test);

	//print statements for testing user/password input
	//printf("\n%s", username);
	//printf("\n%s", password);


}
