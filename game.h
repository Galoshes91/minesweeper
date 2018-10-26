#ifndef __GAME_H__
#define __GAME_H__

extern char gameBoardString[1024];
extern char leaderboard[1024];


// reads from authentication.txt and stores at in server.c as a data structure
void drawBoard(void);

// places the mines on the tile structs
void placeMines(void);

// draws the game menu
int drawMenu(void);

// main entry/running function to process information
void runGame(void);

// initial board setup, placing the mines and setting the adjacent mines value
void setupBoard(void);

// sets the tile to "revealed"
void revealTile(int rowNum, int colNum);

// sets the tile to flagged
void flagTile(int rowNum, int colNum);

// processes the game board
void processGame(char option[], int row, int column);

//draws the leaderboard
void drawLeaderboard(void);

// updates the leaderboard with the given values
void updateLeaderboard(char user[], int time, int wonGames, int played);

#endif //__SERVER_H__