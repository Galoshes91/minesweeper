#ifndef __CLIENT_H__
#define __CLIENT_H__


// function for authenticating user credentials.
int authentication(void);

// draws main menu providing authentication is successful
int mainMenu(void);

// draws minesweeper game board (minus mines)
int drawBoard(void);

#endif  //__CLIENT_H__
