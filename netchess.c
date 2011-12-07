#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server/ncserver.h"
#include "board.h"

#define MAX_TOKENS 3
#define TOKEN_LENGTH 32
#define MAX_INPUT_LENGTH 96

const char* commands[] = {"q", "help", "board", "move", "turn", 0};

const char* team_names[] = {"White", "Black"};

int conntype=0; //0 for standalone, 1 as server, 2 as client

static void tokenize(char* str, char** tokens);
static int cmd_to_idx(char* cmd);

int victory_check(board_t b)
{
	return 0;
}

void print_help()
{
	printf(	"NetChess follows standard chess rules."
		"\nThe game starts with the white team making the first move."

		"\n\nCOMMAND		EFFECT"
		"\nq		Quit"

		"\n\nhelp	\tPrint documentation"

		"\n\nboard	\tPrint the game board"

		"\n\nmove 	\tTakes two chess coordinates as arguments,"
		"\n		and moves a piece if legal"

		"\n\nturn	\tEnds the current player's turn, and"
		"\n		finalizes their moves"

		"\n\n" );
}

void chess_shell(board_t board)
{
	coordinate_t currSrc, currDest;
	coordinate_t oppSrc, oppDest;
	int i, run = 1;
	char* tokens[MAX_TOKENS];
	char  str[MAX_INPUT_LENGTH];

	char team = 0, turn_change = 0;

	board_t temp_board;
	memcpy(temp_board, board, sizeof(piece_t) * (ROWCOL * ROWCOL));

	for(i = 0; i < MAX_TOKENS; i++) tokens[i] = malloc(TOKEN_LENGTH + 1);

	printf("Game starts with the white(+) team.\n");

	while(run)
	{
		if(turn_change)
		{
			printf("%s team moves\n: ", team_names[team]);
			turn_change = 0;
			if(conntype==TYPE_SERVER){
				flipTurn(); //flipTurn() for server; black's turn
				getOpponentMove(&oppSrc,&oppDest); //getOpponentMove(2) for move of opponent: blocks here
				//set the board according to opponent moves
				memcpy(temp_board, board, sizeof(piece_t) * (ROWCOL * ROWCOL));
				board_move(temp_board, team, oppSrc, oppDest); //black's move made on temporary board
				//make changes to board permanently and allow server/white to move again
				memcpy(board, temp_board, sizeof(piece_t) * (ROWCOL * ROWCOL));
				team ^= 1, turn_change = 1;
				//printf("Opponent moved");
			} 

			
		}
			 else printf(": ");

		for(i = 0; i < MAX_TOKENS; i++) memset(tokens[i], 0, TOKEN_LENGTH);

		fgets(str, MAX_INPUT_LENGTH, stdin);
		tokenize(str, (char**)tokens);

		switch(cmd_to_idx(change_case(0, tokens[0])))
		{
			case -0x1:
				printf("\n\"%s\" is not a valid command. Type \"help\" for instructions.\n", tokens[0]);
			break;
			case  0x0:
				run = 0;
			break;
			case  0x1:
				print_help();
			break;
			case  0x2:
				board_print(temp_board);
			break;
			case  0x3:
				memcpy(temp_board, board, sizeof(piece_t) * (ROWCOL * ROWCOL));
				board_move(temp_board, team, tokens[1], tokens[2]);
				currSrc[0]=tokens[1][0], currSrc[1]=tokens[1][1], currDest[0]=tokens[2][0], currDest[1]=tokens[2][1]; //store src, dest coordinate_t for transfer
			break;
			case  0x4:
				memcpy(board, temp_board, sizeof(piece_t) * (ROWCOL * ROWCOL));
				team ^= 1, turn_change = 1;
				if(conntype==TYPE_SERVER){
					movePiece(currSrc,currDest);
				}
//movePiece(2)
			break;
		};
	}

	for(i = 0; i < MAX_TOKENS; i++) free(tokens[i]);
}

int main(int argc, char** argv)
{
if(argc==2){
if(argv[1][0]=='0'){ conntype=TYPE_STANDALONE; printf("Standalone mode\n");}
if(argv[1][0]=='1'){ conntype=TYPE_SERVER; printf("Server mode\n");}	
//set for client mode
}else{
printf("netchess <opt>\n<opt>=0 for standalone\n<opt>=1 for server\n");
return -1;
}
board_t b;
board_init(b);
//initialize server/client here
switch(conntype){
case TYPE_SERVER:
startServer();
break;
default:
break;
}
//check if connection/match is ready
switch(conntype){
case TYPE_SERVER:
if(isMatchSet()==OK) printf("Match set\n");
break;
default:
break;
}
chess_shell(b);

return 0;
}

void tokenize(char* str, char** tokens)
{
	int i = 0;
	char* token = strtok(str, "     \n");

	for(i = 0; token && (i < MAX_TOKENS); i++)
	{
		strcpy(tokens[i], token);
		token = strtok(NULL, "  \n");
	}
}

// Convert a text command to a corresponding instruction index
int cmd_to_idx(char* cmd)
{
	int i;
	for(i = 0; commands[i]; i++)
		if(strcmp(cmd, commands[i]) == 0) return i;

	return -1;
}

