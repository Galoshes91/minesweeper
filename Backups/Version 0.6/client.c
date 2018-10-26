#define _GNU_SOURCE
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
int network_socket;

char * username = "";
char * password = "";

char * reqLogin = "REQ LOGIN";
char * reqLogout = "REQ LOGOUT";
char * response;
bool runningGame;


// Get response from client/server
char * receiveResponse( int socket ) {

	char buffer[1024];

	int bytes = recv( socket, buffer, sizeof( buffer ), 0 );

	buffer[bytes] = '\0';

	char * response = malloc( sizeof(char) * strlen( buffer ) );

	strcpy( response, buffer );

	return response;
}

char * getInput( char * prompt ) {

	printf( "%s", prompt );

	char buffer[1024];

	fgets( buffer, sizeof( buffer ), stdin );

	buffer[ strlen( buffer ) - 1 ] = '\0';

	char * message = NULL;

	message = malloc( sizeof( char ) * strlen( buffer ) );

	strcpy( message, buffer );

	return message;
}

int mainMenu(void) {

	int menuOption;

	printf("\n\nWelcome to the Minesweeper gaming system.\n\n");

	do {

		printf("Please enter a section: \n");
		printf("<1> Play Minesweeper\n<2> Show Leaderboard\n<3> Quit\n<4> Send message\n\n");

		char * end;
		menuOption = strtol( getInput( "Selection option (1-4): " ), &end, 10 );

		if( ( menuOption < 1 ) || ( menuOption > 4 ) ) printf("%sInvalid choice%s\n\n", TITLEBORDER, TITLEBORDER);

	} while( ( menuOption < 1 ) || ( menuOption > 4 ) );

	return menuOption;


}

void clientShutdown( void ) {

	close( network_socket );
}

int main(int argc, char *argv[]) {
	
	atexit( clientShutdown );

	int menuChoice;

	//create the socket
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	//connect to the server
	int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	
	//check the connection status (-1 means failed connection)
	if(connection_status == -1) {
		
		printf("Error connecting...\n");
	}
	
	
	/*
	
	Game code
	
	*/
	
	printf("%s\n \
Welcome to the online Minesweeper \
gaming system\n%s\n\n", TITLEBORDER, TITLEBORDER);



	//authentication();
	printf("You are required to log on with your registered user\
name and password.\nMake sure your username and password are not nothing when entering.\n\n");

	bool loggedIn = false;

	while ( !loggedIn ) { 

		send( network_socket, reqLogin, strlen( reqLogin ), 0 );

		response = receiveResponse( network_socket );

		if ( strcmp( response, "REQ CREDENTIALS" ) == 0 ) { 

			while ( strlen( username ) < 1 ) { username = getInput( "Username: " ); }

			send( network_socket, username, strlen(username), 0 );
			
			while ( strlen( password ) < 1 ) { password = getInput( "Password: " ); }
			
			send( network_socket, password, strlen( password ), 0 );

		}
		
		response = receiveResponse( network_socket );

		if ( strcmp( response, "LOGIN SUCCESSFUL" ) == 0 ) {

			printf( "\n%s", "Successfully logged in." );
			loggedIn = true;

		} else if ( strcmp( response, "USER ALREADY LOGGED IN" ) == 0 ) {

			printf( "\n%s%s%s\n%s\n\n",
					"User '",
					username,
					"' already logged in.",
					"Exiting..." );
			
			close(network_socket);
			exit( 1 );

		} else if ( strcmp( response, "USER DOES NOT EXIST" ) == 0 ) {

			printf( "\n%s%s%s\n%s\n\n",
					"User '",
					username,
					"' does not exist.",
					"Exiting..." );

			close(network_socket);
			exit( 1 );

		} else if ( strcmp( response, "PASSWORD INCORRECT" ) == 0 ) {

			printf( "\n%s\n%s\n\n",
					"Password incorrect.",
					"Exiting..." );
			close(network_socket);
			exit( 1 );
		}

		sleep( 1 );
	}

	while(loggedIn) {

		menuChoice = mainMenu();

		if (menuChoice == 1) {

			send(network_socket, "WTP", 3, 0);

			//response = receiveResponse(network_socket);

			//printf("%s", response);

			runningGame = true;

			char * selectedOption;
			char * chosenRow;
			char * chosenCol; 

			while (runningGame) {

				response = receiveResponse(network_socket);


				printf("%s", response);

				selectedOption = getInput("Option (R,P,Q): ");

				send(network_socket, selectedOption, sizeof(selectedOption), 0);


				if (strcmp(selectedOption, "q") == 0) {

					runningGame = false;

				}

				else if ((strcmp(selectedOption, "r") == 0) || (strcmp(selectedOption, "R") == 0 )) {
					chosenRow = getInput("Select row: ");
					chosenCol = getInput("Select column: ");

					send(network_socket, chosenRow, sizeof(chosenRow), 0);
					send(network_socket, chosenCol, sizeof(chosenCol), 0);
				}

				else if ((strcmp(selectedOption, "p") == 0) || (strcmp(selectedOption, "P") == 0)) {
					chosenRow = getInput("Select row: ");
					chosenCol = getInput("Select column: ");

					send(network_socket, chosenRow, sizeof(chosenRow), 0);
					send(network_socket, chosenCol, sizeof(chosenCol), 0);
				}

			}

		}

		else if (menuChoice == 2) {

			send(network_socket, "LDB", 3, 0);

			response = receiveResponse(network_socket);

			printf("%s\n", response);



		}
		
		else if ( menuChoice == 3 ) {

			bool loggedOut = false;

			while ( !loggedOut ) { 

				send( network_socket, reqLogout, strlen( reqLogout ), 0 );

				response = receiveResponse( network_socket );

				if ( strcmp( response, "REQ CREDENTIALS" ) == 0 ) { 

					send( network_socket, username, strlen(username), 0 );
				}

				response = receiveResponse( network_socket );

				if ( strcmp( response, "LOGOUT SUCCESSFUL" ) == 0 ) {

					printf( "\n%s\n\n", "Successfully logged out." );
					loggedOut = true;

				} else if ( strcmp( response, "USER ALREADY LOGGED OUT" ) == 0 ) {

					printf( "\n%s%s%s\n%s\n\n",
							"User '",
							username,
							"' already logged out.",
							"Exiting..." );

					close(network_socket);
					exit( 1 );

				} else if ( strcmp( response, "USER DOES NOT EXIST" ) == 0 ) {

					printf( "\n%s%s%s\n%s\n\n",
							"User '",
							username,
							"' does not exist.",
							"Exiting..." );

					close(network_socket);
					exit( 1 );

				}
			}
		} else if ( menuChoice == 4 ) {

			while ( true ) {

				char * message = getInput( "Enter message: ");

				if ( strcmp( message, "exit" ) == 0 ) {
					break;
				} else {
					send( network_socket, message, strlen( message ), 0 );
				}
			}

		} else {
			//Do nothing
		}

	}

	close(network_socket);
	return 0;
}
