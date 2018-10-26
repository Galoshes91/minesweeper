
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
	
	//listens for connections
	listen(server_socket, 5);

	bool loggedIn = false;

	// While loop to accept connections, read the data and process it appropriately 

	while(true) {

		//test();

		int client_socket;
		client_socket = accept(server_socket, NULL, NULL);

		// client's data being sent
		char clientData[256];


		recv(client_socket, &clientData, sizeof(clientData), 0);

		printf("%s\n", clientData);

		// Checks if the user is logged in
		if(!loggedIn) {

			loggedIn = readAccounts(clientData);
		}
		


		// Runs the game, provided the user is logged in
		if(loggedIn) {
			int loginTest = 1;
			send(client_socket, &loginTest, sizeof(int), 0);
			placeMines();
			drawBoard();

			printf("%s\n", sendingBoard);

			send(client_socket, &gameBoardString, sizeof(gameBoardString), 0);

			//free(gameBoardString);
		}


		// essential code for loop iterations 

		sleep(1);

		close(client_socket);

	}

	return 0;

}