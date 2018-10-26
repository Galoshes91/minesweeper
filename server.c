#define _GNU_SOURCE
#include "game.h"
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <stdbool.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>
#include <time.h>


#define RANDOM_NUMBER_SEED 42
#define BACKLOG 10     /* how many pending connections queue will hold */
#define RETURNED_ERROR -1

int server_socket;
int client_socket;

typedef struct Account {
	char * username;
	char * password;
	bool loggedIn;
} Account;

Account ** accountList;
int accountNum = 0;

char LDBUsername[64];
time_t startTime;
time_t endTime;
int playCounter = 0;
int winCounter = 0;

char * reqCredentials = "REQ CREDENTIALS";
char * loginSuccessful = "LOGIN SUCCESSFUL";
char * logoutSuccessful = "LOGOUT SUCCESSFUL";
char * userLoggedIn = "USER ALREADY LOGGED IN";
char * userLoggedOut = "USER ALREADY LOGGED OUT";
char * userInvalid = "USER DOES NOT EXIST";
char * passwordIncorrect = "PASSWORD INCORRECT";
bool runningGame;


void readAccounts(void) {

	FILE * accountsFile = fopen( "Authentication.txt", "r" );
	char * account;
	size_t fileLength = 0;
	ssize_t lineSize;
	int currentAccount = 0;

	if ( accountsFile == NULL ) exit( 1 );

	while ( ( lineSize = getline( &account, &fileLength, accountsFile ) ) != EOF ) {

		if ( currentAccount > 0 ) {

			char * username = strtok( account, "  	" );
			char * password;

			char * test = username;
			while ( test != NULL ) {
				password = test;
				test = strtok( NULL, "	" );
			}

			accountList = realloc( accountList, sizeof( Account ) * ( accountNum + 1 ) );
			accountList[ accountNum ] = malloc( sizeof( Account ) );

			accountList[ accountNum ] -> username = malloc( strlen( username ) + 1 );
			strcpy( accountList[ accountNum ] -> username, username );

			if ( password[ strlen( password ) - 1 ] == '\n' ) {
				password[ strlen( password ) - 1 ] = '\0';
			}

			accountList[ accountNum ] -> password = malloc( strlen( password ) + 1 );
			strcpy( accountList[ accountNum ] -> password, password );

			accountList[ accountNum ] -> loggedIn = false;

			accountNum++;
		}

		currentAccount++;
	}

	free( account );

	fclose( accountsFile );
}

// Get response from client/server
char * receiveResponse( int socket ) {

	char buffer[1024];

	int bytes = recv( socket, buffer, sizeof( buffer ), 0 );

	buffer[bytes] = '\0';

	char * response = malloc( sizeof(char) * strlen( buffer ) );

	strcpy( response, buffer );

	return response;
}


void setup( int argc, char* argv[] ) {

	srand(RANDOM_NUMBER_SEED);
	
	//create the server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//specify the address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	// bind the socket to our specified IP and port
	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
}

void runServer() {

	printf("Started Server...\n\n");

	//listens for connections
	listen(server_socket, 5);

	bool killServer = false;

	//accept the connection from the client
	client_socket = accept(server_socket, NULL, NULL);

	while ( !killServer ) {

		char * response = receiveResponse( client_socket );

		if ( strlen( response ) > 0 ) printf( "[MESSAGE] %s\n", response );


		// LOGIN CLIENT
		if ( strcmp( response, "REQ LOGIN" ) == 0 ) {

			printf( "[LOGIN] Login Requested... Waiting for login data...\n" );
			send( client_socket, reqCredentials, strlen( reqCredentials ), 0 );

			bool loggedIn = false;

			while ( !loggedIn ) {

				char * username = receiveResponse( client_socket );
				char * password = receiveResponse( client_socket );

				for ( int currentAccount = 0; currentAccount < accountNum; currentAccount++ ) {

					if ( strcmp( username, accountList[ currentAccount ] -> username ) == 0 ) {

						if ( accountList[ currentAccount ] -> loggedIn == true ) {

							printf("%s%s%s\n",
								   "[LOGIN] User '",
								   username,
								   "' already logged in.");

							send( client_socket, userLoggedIn, strlen( userLoggedIn ), 0 );

							loggedIn = true;
							break;
						}

						if ( strcmp( password, accountList[ currentAccount ] -> password ) == 0 ) {
							
							printf("%s%s%s\n",
								   "[LOGIN] Successfully logged in user '",
								   username,
								   "'.");

							strcpy(LDBUsername, username);

							accountList[ currentAccount ] -> loggedIn = true;
							send( client_socket, loginSuccessful, strlen( loginSuccessful ), 0 );


							loggedIn = true;
							break;

						} else { 

							printf("%s%s%s\n",
							  	   "[LOGIN] Password for '",
								   username,
								   "' is incorrect." );

							send( client_socket, passwordIncorrect, strlen( passwordIncorrect ), 0 );
							loggedIn = true;
							break;
						}

					} else if ( currentAccount == ( accountNum - 1 ) ) {

						printf("%s%s%s\n",
							   "[LOGIN] User '",
							   username,
							   "' does not exist." );
						send( client_socket, userInvalid, strlen( userInvalid ), 0 );
						loggedIn = true;
					}
				}

				sleep( 1 );
			}
		}

		if (strcmp (response, "WTP") == 0) {

			playCounter++;

			time(&startTime);

			setupBoard();

			drawBoard();

			send(client_socket, gameBoardString, sizeof(gameBoardString), 0);

			runningGame = true;

		}

		if (strcmp(response, "LDB") == 0) {

			drawLeaderboard();

			send(client_socket, leaderboard, sizeof(leaderboard), 0);


		}

		if( (strcmp(response, "r") == 0) && (runningGame)) {

			int rowNum;
			int colNum;

			char * chosenRow = receiveResponse(client_socket);
			char * chosenCol = receiveResponse(client_socket);

			rowNum = atoi(chosenRow);
			colNum = atoi(chosenCol);


			processGame("r", rowNum, colNum);

			drawBoard();

			send(client_socket, gameBoardString, sizeof(gameBoardString), 0);

			//printf("%s\n", chosenRow);

			//printf("%s\n", chosenCol);

		}

		if( (strcmp(response, "p") == 0) && (runningGame)) {

			int rowNum;
			int colNum;

			char * chosenRow = receiveResponse(client_socket);
			char * chosenCol = receiveResponse(client_socket);

			rowNum = atoi(chosenRow);
			colNum = atoi(chosenCol);


			processGame("p", rowNum, colNum);

			drawBoard();

			send(client_socket, gameBoardString, sizeof(gameBoardString), 0);

		}

		if ( (strcmp(response, "q") == 0) && (runningGame)) {

			time(&endTime);

			int totalTime = (int) endTime - startTime;



			runningGame = false;

			updateLeaderboard(LDBUsername, totalTime, winCounter, playCounter);
		}


		// LOGOUT USER
		if ( strcmp( response, "REQ LOGOUT" ) == 0 ) {

			printf( "[LOGOUT] Logout Requested... Waiting for login data...\n" );
			send( client_socket, reqCredentials, strlen( reqCredentials ), 0 );

			bool loggedOut = false;

			while ( !loggedOut ) {

				char * username = receiveResponse( client_socket );

				for ( int currentAccount = 0; currentAccount < accountNum; currentAccount++ ) {

					if ( strcmp( username, accountList[ currentAccount ] -> username ) == 0 ) {

						if ( accountList[ currentAccount ] -> loggedIn == false ) {

							printf("%s%s%s\n",
								   "[LOGOUT] User '",
								   username,
								   "' already logged out.");

							send( client_socket, userLoggedOut, strlen( userLoggedOut ), 0 );

							loggedOut = true;
							break;
						} else {

							printf("%s%s%s\n",
								   "[LOGOUT] Successfully logged out user '",
								   username,
								   "'.");

							accountList[ currentAccount ] -> loggedIn = false;
							send( client_socket, logoutSuccessful, strlen( logoutSuccessful ), 0 );
							
							loggedOut = true;

							break;
						}

					} else if ( currentAccount == ( accountNum - 1 ) ) {

						printf("%s%s%s\n",
							   "[LOGOUT] User '",
							   username,
							   "' does not exist." );

						send( client_socket, userInvalid, strlen( userInvalid ), 0 );
						loggedOut = true;
					}
				}

				sleep( 1 );
			}

		}

		// SHUTDOWN SERVER
		if ( strcmp( response, "KILL SERVER" ) == 0 ) {
			printf( "%s%s%s\n%s\n", 
					"[SERVER] Received command '",
					response,
					"'",
					"[SERVER] Shutting down..." );

			killServer = true;
		}


		sleep( 1 );
	}

}

void checkAccounts() {

	for ( int currentAccount = 0; currentAccount < accountNum; currentAccount++ ) {
		printf( "Username: %s\nPassword: %s\n\n",
				accountList[ currentAccount ] -> username,
				accountList[ currentAccount ] -> password );
	}	
}

void shutdownAccounts() {

	for ( int currentAccount = 0; currentAccount < accountNum; currentAccount++ ) {

				free( accountList[ currentAccount ] -> username );
				free( accountList[ currentAccount ] -> password );
				free( accountList[ currentAccount ] );
	}

	free( accountList );
}

void serverShutdown( void ) {

	close( server_socket );
	shutdownAccounts();
}

int main( int argc, char* argv[] ) {

	atexit( serverShutdown );
	readAccounts();
	//checkAccounts();
	setup( argc, argv );
	runServer();
	
	//close and finish the server
	close(server_socket);
	
	return 0;

}
