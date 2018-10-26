
#include "game.h"
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
#include <stdbool.h>

#define RANDOM_NUMBER_SEED 42
#define NUM_ROWS 9
#define NUM_COL 9
#define NUM_MINES 10



// Global variables for the game

char userTest[] = "admin";
char passTest[] = "root";
int socketTest;
char sendingBoard[] = "Sending Game Board...";


bool readAccounts(char str[]) {

	/*


		Will need to expand on to read from authentication.txt


	*/

	if(strcmp(userTest, str) == 0) {

		return true;

	}

	else {

		return false;

	}
}





int main(int argc, char* argv[]) {

	//


	if(argc != 2) {

		printf("usage: ./server [port to connect to]\n\n");
		return EXIT_FAILURE;

	}

	srand(RANDOM_NUMBER_SEED);

	int server_socket;
	socketTest = server_socket;
	char clientUsername[256];
	char clientPassword[256];
	int loginSuccess;
	
	//create the server socket
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//specify the address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	// bind the socket to our specified IP and port
	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	

	bool loggedIn = false;

	// While loop to accept connections, read the data and process it appropriately 

	while(true) {

		//test();	
				printf("1");
		//listens for connections
		listen(server_socket, 5);

		int client_socket;
		client_socket = accept(server_socket, NULL, NULL);
		printf("2");
		// client's data being sent
		char clientData[256];
		printf("3");

		recv(client_socket, &clientData, sizeof(clientData), 0);

		printf("%s\n", clientData);
		printf("4");
		// Checks if the user is logged in
		if(!loggedIn) {

			loggedIn = readAccounts(clientData);
		}


				printf("5");

		
		// Runs the game, provided the user is logged in

		int loginTest = 1;

		send(client_socket, &loginTest, sizeof(int), 0);
		printf("6");
		setupBoard();
		drawBoard();

		printf("%s\n", gameBoardString);

		printf("%s\n", sendingBoard);
		printf("7");
		send(client_socket, &gameBoardString, sizeof(gameBoardString), 0);

//		char clientData2[256];
		printf("8");
		//recv(client_socket, &clientData, sizeof(clientData), 0);

		//printf("%s\n", clientData);


// essential code for loop iterations 

		close(client_socket);
						printf("9");
		sleep(1);

	}

	return 0;

}