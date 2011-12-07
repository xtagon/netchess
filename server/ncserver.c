#include "linsock.h"
#include "ncserver.h"


int state;
int sock;
int conn;
int turn;
size_t len;
struct sockaddr_in servaddr;
struct sockaddr_in cliaddr;

int startServer(){
state=STATE_CONNECTING;
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(CHESSPORT);
//socket
if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
fprintf(stderr,"Error creating socket\n");
return ERROR;
}
//bind
if(bind(sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
fprintf(stderr,"Bind error\n");
return ERROR;
}
//listen
if(listen(sock,QUEUESIZE)<0){
fprintf(stderr,"Listening error\n");
return ERROR;
}
state=STATE_READY;
len=sizeof(cliaddr);
if((conn=accept(sock,(struct sockaddr*)&cliaddr,&len))<0){ //at present version only one connection per run
fprintf(stderr,"Connection error\n");
return ERROR;
}
state=STATE_CONNECTED;
turn=TEAM_WHITE;
return OK;
}

int isMatchSet(){
if(state==STATE_CONNECTED)
return OK;
return ERROR;
}

int movePiece(coordinate_t src, coordinate_t dest){
char buff[5]; int w;

if(state==STATE_CONNECTED){
if(turn==TEAM_WHITE){
buff[0]=src[0]; buff[1]=src[1];
buff[2]=dest[0]; buff[3]=dest[1];
buff[4]='\0';
w=write(conn,buff,5); //TODO: Write fault tolerant write(3) routine
flipTurn();
return OK;
}
}
return ERROR;
}

int getOpponentMove(coordinate_t *oppsrc, coordinate_t *oppdest){
char buff[5]; int r;
if(state==STATE_CONNECTED){
if(turn==TEAM_BLACK){
r=read(conn,buff,5); //blocks here and waits for the opponent to make his turn
if(r>=4){
*oppsrc[0]=buff[0]; *oppsrc[1]=buff[1];
*oppdest[0]=buff[2]; *oppdest[1]=buff[3];
}
//done reading
}
}
return ERROR;
}

int getTurn(){
return turn;
}

void flipTurn(int t){
turn^=1;
}

