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

// is isRevealed necessary?

typedef struct  {

	bool isMine;
	bool isRevealed;
	bool isFlagged;
	int adjacentMines;	

} Tile;

Tile gameBoard[NUM_ROWS][NUM_COL];
char gameBoardString[1024];
bool gameOver = false;
char inGameMenu[] = "\nChoose an option:\n<R> Reveal tile\n<P> Place flag\n<Q> Quit game\n\n";
char inGameMenuError[] = "Invalid choice";


void drawBoard(void) {

	//char *gameBoardString = (char *)malloc(sizeof(char) * (NUM_ROWS * NUM_COL));

	//char menuOption[0];
	int menuOption;
	
	strcpy(gameBoardString, "");
	strcat(gameBoardString, "\n");
	// 9 rows, 8 columns
	strcat(gameBoardString, "  1 2 3 4 5 6 7 8 9\n--------------------\n");

	int rowLetter = 65;


	for(int row = 0; row < NUM_ROWS; row ++) {

		char c = rowLetter;
		char d[2];

		d[0] = c;

		d[1] = '\0';

		strcat(gameBoardString, d);
		//strcat(gameBoardString, c);

		for(int column = 0; column < NUM_COL; column ++) {
			
			if(gameBoard[row][column].isMine) {
				strcat(gameBoardString, " *");
			}

			else if (gameBoard[row][column].isFlagged) {
				strcat(gameBoardString, " +");
			}



			else {
				strcat(gameBoardString, "  ");
			}


		}

		strcat(gameBoardString, "\n");
		rowLetter++;

	}

	strcat(gameBoardString, inGameMenu);

}

void placeMines(void) {


	for (int i = 0; i <= NUM_MINES -1; i++) {

			int x, y;
			x = rand() % NUM_ROWS;
			y = rand() % NUM_COL;

			printf("%d, ", x+1);
			printf("%d\n", y+1);

			gameBoard[x][y].isMine = true;

	}

}

void setupBoard(void) {

	placeMines();

	for (int row = 0; row < NUM_ROWS; row ++) {


		printf("  ");
		for (int column = 0; column < NUM_COL; column ++) {

			int adj = 0;

			//checks if the particular tile has any adjacent mines and increases the value
			// by one if it's either on above/below/left/right/diagonal 

			if(!gameBoard[row][column].isMine) {

				

				//checks above the current tile
				if((row != 0) && (gameBoard[row-1][column].isMine)) adj++;
				
				//checks below the current tile 
				if ((row != NUM_ROWS) && (gameBoard[row+1][column].isMine)) adj++;

				//checks to the left of the tile 
				if((column != 0) && gameBoard[row][column-1].isMine) adj++;

				//checks to the right of the tile
				if((column != NUM_COL) && gameBoard[row][column+1].isMine) adj++;

				//checks to the NE of the current tile
				if((column != NUM_COL) && (row != 0) && (gameBoard[row-1][column+1].isMine)) adj++;

				//checks to the NW of the current tile
				if((column != 0) && (row != 0) && (gameBoard[row-1][column-1].isMine)) adj++;

				//checks to the SE of the current tile 
				if((column != NUM_COL) && (row != NUM_ROWS) && (gameBoard[row+1][column+1].isMine)) adj++;

				//checks to the SW of the current tile
				if((column != 0) && (row != NUM_ROWS) && (gameBoard[row+1][column-1].isMine)) adj++;
			}

			//char adjChar = '0' + adj;

			gameBoard[row][column].adjacentMines = adj;

		}

	}
}

void revealTile(int rowNum, int colNum) {

	gameBoard[rowNum-1][colNum-1].isRevealed = true;

}

void flagTile(int rowNum, int colNum) {

	gameBoard[rowNum-1][colNum-1].isFlagged = true;
}


/* ALL MINES REVEALED FOR TESTING PURPOSES

2, 8
1, 8
6, 8
2, 4
7, 2
6, 5
6, 8
6, 5
7, 1
8, 2

  1 2 3 4 5 6 7 8 9
--------------------
A               *
B       *       *
C
D
E
F         *     *
G * *
H   *
I

*/



int main(void) {

	setupBoard();

	drawBoard();


	printf("%s\n", gameBoardString);

	return 0;

}