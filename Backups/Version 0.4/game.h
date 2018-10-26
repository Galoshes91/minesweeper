#ifndef __GAME_H__
#define __GAME_H__

extern char gameBoardString[1024];
// reads from authentication.txt and stores at in server.c as a data structure
void drawBoard(void);

void placeMines(void);

int drawMenu(void);

void runGame(void);

#endif //__SERVER_H__