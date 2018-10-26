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
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdbool.h>

#define MAXDATASIZE 100 /* max number of bytes we can get at once */
#define TITLEBORDER "================================================"


int socketNumber;

//char *password;


int drawBoard(void) {


	//char menuOption[0];
	int menuOption;
	

	// 9 rows, 8 columns
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
	
	//if (menuOption == 1) drawBoard();

	return menuOption;


}

int main(int argc, char *argv[]) {
	char username[100];
	char password[100];
	
	
	//int menuChoice;
	
	if(argc != 2) {

		printf("usage: ./client [port to connect to]\n\n");
		return EXIT_FAILURE;
	}



	//create the socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	//connect to the server
	int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	
	//check the connection status (-1 means failed connection)
	if(connection_status == -1) {
		
		printf("Error connecting...\nExiting...\n\n");
		return EXIT_FAILURE;
	}
	

		printf("%s\n \
Welcome to the online Minesweeper \
gaming system\n%s\n\n", TITLEBORDER, TITLEBORDER);



	//authentication();
	printf("You are required to log on with your registered user\
name and password.\n");

	printf("Username: ");
	scanf("%s", username);
	//printf("%c")
	send(network_socket, username, sizeof(username), 0);
	

	int loginTest;
	recv(network_socket, &loginTest, sizeof(int), 0);



	if(loginTest == 1) {

		char gameBoard[1024];

		recv(network_socket, &gameBoard, sizeof(gameBoard), 0);

		printf("%s\n", gameBoard);
		//printf("Login Success, exiting...\n");
		//return EXIT_SUCCESS;
	}

	else {
		printf("Login Failure, exiting...\n");
		return EXIT_FAILURE;

	}


	close(network_socket);

	return 0;
}	