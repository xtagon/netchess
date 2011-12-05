all: board.o netchess

board.o: board.c
	gcc -c board.c

netchess: board.o netchess.c
	gcc -onetchess board.o netchess.c
