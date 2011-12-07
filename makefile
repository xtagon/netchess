all: board.o netchess

board.o: board.c
	gcc -c board.c

netchess: server/ncserver.o board.o netchess.c
	gcc -onetchess server/ncserver.o board.o netchess.c

