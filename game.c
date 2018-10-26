#define _GNU_SOURCE
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
	int adjacentMines;	

} Tile;

typedef struct {
	char userName[64];
	int timePlayed;
	int gamesWon;
	int gamesPlayed;

} userDetails;

Tile gameBoard[NUM_ROWS][NUM_COL];
userDetails leaderboardArray[20];

char gameBoardString[1024];
char leaderboard[1024];
bool gameOver = false;
char inGameMenu[] = "\nChoose an option:\n<R> Reveal tile\n<P> Place flag\n<Q> Quit game\n\n";
char inGameMenuError[] = "Invalid choice";

int LDBcounter = 0;
char loggedIn[64];
int timer = 0;
int gamesWon = 0;
int gamesPlayed = 0;

void drawLeaderboard(void) {
	

	char temp[256];



	strcpy(leaderboard, "");

	strcat(leaderboard, "\n");

	strcat(leaderboard, "==== LEADERBOARD ====\n\n");

	strcat(leaderboard, TITLEBORDER);

	strcat(leaderboard, "\n\n");

	for (int i = 0; i < LDBcounter; i++) {

		if(leaderboardArray[i].timePlayed > 0) {

			sprintf(temp, "%s     %d Seconds      %d games won, %d games played", leaderboardArray[i].userName, leaderboardArray[i].timePlayed, leaderboardArray[i].gamesWon, leaderboardArray[i].gamesPlayed);
			strcat(leaderboard, temp);
			strcat(leaderboard, "\n\n");
		}

	}

	
	strcat(leaderboard, "\n\n");
	
	strcat(leaderboard, TITLEBORDER);

	strcat(leaderboard, "\n\n");



}

void updateLeaderboard(char user[], int time, int wonGames, int played) {

	bool userExists = false;
	int iValue;

	if(LDBcounter < 20) {

		for (int i = 0; i <= LDBcounter; i++) {
			if(strcmp(leaderboardArray[i].userName, user) == 0 )  {
				userExists = true;
				iValue = i;

			}

		}

		if(userExists) {
			leaderboardArray[iValue].timePlayed = leaderboardArray[iValue].timePlayed + time;
			leaderboardArray[iValue].gamesWon = wonGames;
			leaderboardArray[iValue].gamesPlayed = played;
		}
		else {
			strcpy(leaderboardArray[LDBcounter].userName, user);
			leaderboardArray[LDBcounter].timePlayed = time;
			leaderboardArray[LDBcounter].gamesWon = wonGames;
			leaderboardArray[LDBcounter].gamesPlayed = played;
		}

		LDBcounter++;
	}

	else {
		LDBcounter = 0;
		updateLeaderboard(user, time, wonGames, played);
	}



}

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


			char str[5];

			sprintf(str, "%d" ,gameBoard[row][column].adjacentMines);


			
			if((gameBoard[row][column].isMine) && (gameBoard[row][column].isRevealed)) {
				strcat(gameBoardString, " *");
				// need gameover code here

			}

			else if ((gameBoard[row][column].isFlagged)) {
				strcat(gameBoardString, " +");
			}


			else if ((gameBoard[row][column].isRevealed)) {// && (!gameBoard[row][column].isFlagged) && (!gameBoard[row][column].isRevealed)) {
				strcat(gameBoardString, " ");
				strcat(gameBoardString, str);
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


			gameBoard[x][y].isMine = true;

	}

}

void setupBoard(void) {

	placeMines();

	for (int row = 0; row < NUM_ROWS; row ++) {
		for (int column = 0; column < NUM_COL; column ++) {

			gameBoard[row][column].isRevealed = false;
			//gameBoard[row][column].isRevealed = true;

		}
	}



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
				if ((row < NUM_ROWS - 1) && (gameBoard[row+1][column].isMine)) adj++;

				//checks to the left of the tile 
				if((column != 0) && gameBoard[row][column-1].isMine) adj++;

				//checks to the right of the tile
				if((column < NUM_COL - 1) && gameBoard[row][column+1].isMine) adj++;

				//checks to the NE of the current tile
				if((column < NUM_COL - 1) && (row != 0) && (gameBoard[row-1][column+1].isMine)) adj++;

				//checks to the NW of the current tile
				if((column != 0) && (row != 0) && (gameBoard[row-1][column-1].isMine)) adj++;

				//checks to the SE of the current tile 
				if((column < NUM_COL - 1) && (row < NUM_ROWS - 1) && (gameBoard[row+1][column+1].isMine)) adj++;

				//checks to the SW of the current tile
				if((column != 0) && (row < NUM_ROWS -1 ) && (gameBoard[row+1][column-1].isMine)) adj++;
			}

			//char adjChar = '0' + adj;

			gameBoard[row][column].adjacentMines = adj;

		}

	}
}

void revealTile(int rowNum, int colNum) {

	gameBoard[rowNum-1][colNum-1].isRevealed = true;

	// to do : figure out recursion of 0 adj mines

}

void flagTile(int rowNum, int colNum) {

	gameBoard[rowNum-1][colNum-1].isFlagged = true;
}

void checkAdjMines(int rowNum, int colNum) {

	//checks to the right of the given tile

	if((colNum + 1 != NUM_COL) && (gameBoard[rowNum][colNum].adjacentMines == 0)) { 

			revealTile(rowNum, colNum);
			
			checkAdjMines(rowNum, colNum + 1);


	}
	
	//checks to the left of the given tile
/*
	if ((colNum > 0) && (gameBoard[rowNum][colNum].adjacentMines == 0)) {

		revealTile(rowNum, colNum);

		checkAdjMines(rowNum, colNum - 1);

	}/*

	// checks down of the given tile

	if ((rowNum + 1 != NUM_ROWS) && (gameBoard[rowNum][colNum].adjacentMines == 0)) {

		gameBoard[rowNum][colNum].isRevealed = true;

		checkAdjMines(rowNum + 1, colNum);

	}*/
}


/* ALL MINES REVEALED FOR TESTING PURPOSES


  1 2 3 4 5 6 7 8 9
--------------------
A 0 0 1 1 1 0 2 0 2
B 0 0 1 0 1 0 2 0 2
C 0 0 1 1 1 0 1 1 1
D 0 0 0 0 0 0 0 0 0
E 0 0 0 1 1 1 1 1 2
F 2 2 1 1 0 1 1 0 2
G 0 0 2 1 1 1 1 1 2
H 3 0 2 0 0 0 0 0 0
I 1 1 1 0 0 0 0 0 0


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

void processGame(char option[], int row, int column) {

	if ((strcmp(option, "r") == 0) || (strcmp(option, "R") == 0 )) {
		if (gameBoard[row][column].isMine) {
			gameOver = true;
			revealTile(row, column);

		}

		else {

			revealTile(row, column);

			//if(gameBoard[row][column].adjacentMines == 0) checkAdjMines(row, column);
		}

	}

	else if ((strcmp(option, "p") == 0) || (strcmp(option, "P") == 0 )) flagTile(row, column);


}

/*

int main(void) {

	setupBoard();

	//processGame("r", 4, 1);
	processGame("r", 4, 2);
	processGame("p", 3, 4);
	

	drawBoard();

	printf("%s\n", gameBoardString);

	return 0;

}*/