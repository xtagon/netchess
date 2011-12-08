#ifndef NCSERVER_H
#define NCSERVER_H

#include "../board.h"

#define CHESSPORT 21000
#define OK 0
#define ERROR -1

#define TYPE_STANDALONE 0x00
#define TYPE_SERVER 0x01
#define TYPE_CLIENT 0x02

#define STATE_CONNECTING  0x010
#define STATE_CONNECTED 0x020
#define STATE_READY 0x030
#define STATE_PLAYING 0x040

int startServer(); //start the chess server
int isMatchSet(); //0 if set -1 otherwise
int movePiece(coordinate_t src, coordinate_t dest); //for moving pieces
int getTurn();
void flipTurn();
int getOpponentMove(char *oppsrc, char *oppdest);
#endif
