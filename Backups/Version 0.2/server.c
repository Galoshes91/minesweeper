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


// Struct for the tile objects

typedef struct  {

	bool isMine;	

} Tile;

/*
typedef struct { 

	Tile tiles[NUM_COL][NUM_ROWS];

} gameState;*/

char userTest[] = "admin";
char passTest[] = "root";
int socketTest;

void readAccounts(void) {

	//read from Authentication.txt, save as array?


}

void setupGame(void) {


	Tile rowTest[NUM_COL][NUM_ROWS];

	//printf("%ld\n\n", sizeof(rowTest));

	for (int i = 0; i < NUM_MINES; i++) {

			int x, y;
			x = rand() % NUM_COL;
			y = rand() % NUM_ROWS;

			rowTest[x][y].isMine = true;

	}

	/*

	printf("%d\n\n", rand() % NUM_ROWS);
	printf("%d\n\n", rand() % NUM_ROWS);
	printf("%d\n\n", rand() % NUM_ROWS);*/

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
	//int menuChoice;
	//int server_message = 4;
	
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
	
	//accept the connection from the client
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	

	
	
	//send the data
	recv(client_socket, &clientUsername, sizeof(clientUsername), 0);
	
	recv(client_socket, &clientPassword, sizeof(clientPassword), 0);
	
	/*
	int something;
	recv(client_socket, &something, 3 * sizeof(int), 0);
	*/
	//send(network_socket, &hello, 3 * sizeof(int), 0);
	//printf("%s\n", client_message);
	
	if(strcmp(clientUsername, userTest) == 0 &&
		strcmp(clientPassword, passTest) == 0) {
		
		printf("%s\n", clientUsername);
		printf("%s\n", clientPassword);
		//printf("%d\n", something);
		loginSuccess = 1;
		send(client_socket, &loginSuccess, 3 * sizeof(int), 0);

		int menuChoice;
		recv(client_socket, &menuChoice, 3 *sizeof(int), 0);
		printf("Selected menu choice: %d\n", menuChoice);

		setupGame();

		
	} 
	else {
		printf("Login Failure\n\n");
		loginSuccess = 0;
		send(client_socket, &loginSuccess, 3 * sizeof(int), 0);
		return EXIT_FAILURE;
	}
	
	/* Don't fucking know what's wrong with this
	char testNum[256] = "2";
	
	send(client_socket, &testNum, sizeof(testNum), 0);*/


	//close and finish the server
	close(server_socket);


	return 0;


	//

}
