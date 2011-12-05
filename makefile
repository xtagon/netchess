all: netchess

netchess: board.c netchess.c
	gcc -onetchess board.c netchess.c
