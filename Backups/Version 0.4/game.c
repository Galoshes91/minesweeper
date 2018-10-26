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
#define TITLEBORDER "================================================"

// Struct for the tile objects

typedef struct  {

	bool isMine;
	bool isRevealed;
	bool isFlagged;	

} Tile;

Tile gameBoard[NUM_COL][NUM_ROWS];
char gameBoardString[1024];
bool gameOver = false;


int drawMenu(void) {

	int menuOption;

	do {
	
		printf("\nChoose an option:\n<1> Reveal tile\n<2> Place flag\n<3> Quit game\n\nOption (1,2,3):");
		scanf("%d", &menuOption);
		
		if((menuOption < 1) || (menuOption > 3)) printf("%sInvalid choice%s\n\n", TITLEBORDER, TITLEBORDER);


	} while((menuOption > 3) || (menuOption < 1));
	
	return menuOption;
	
}

void drawBoard(void) {

	//char *gameBoardString = (char *)malloc(sizeof(char) * (NUM_ROWS * NUM_COL));

	//char menuOption[0];
	int menuOption;
	
	strcpy(gameBoardString, "");

	// 9 rows, 8 columns
	strcat(gameBoardString, "   1 2 3 4 5 6 7 8 9\n--------------------\n");

	int rowLetter = 65;

	for(int row = 0; row < NUM_ROWS; row ++) {

		char c = rowLetter;

		//"A |\n"

		strcat(gameBoardString, &c);

		for(int column = 0; column < NUM_COL; column ++) {

			if(gameBoard[column][row].isMine) {
				strcat(gameBoardString, " *");
			}
			else {
				strcat(gameBoardString, "  ");
			}


		}

		strcat(gameBoardString, "\n");
		rowLetter++;

	}

}

void placeMines(void) {


	for (int i = 0; i <= NUM_MINES; i++) {

			int x, y;
			x = rand() % NUM_COL;
			y = rand() % NUM_ROWS;

			gameBoard[x][y].isMine = true;

	}

}

void revealTile(void) {

	//
}

void flagTile(void) {

	//
}


/* ALL MINES REVEALED FOR TESTING PURPOSES

   1 2 3 4 5 6 7 8 9
--------------------
AA             *
BB             * *
CC
DD   *
EE           *
FF
GG
HH * *       *
II                 *	

*/

void runGame(void) {

	placeMines();

	do {

	drawBoard();
	printf("%s\n", gameBoardString);
	int menuOption = drawMenu();

	if(menuOption == 1) revealTile();
	else if (menuOption == 2) flagTile;
	else if (menuOption == 3) gameOver = true;

	} while(!gameOver);

	
}

/*
int main(void) {

	runGame();

	return 0;

}*/