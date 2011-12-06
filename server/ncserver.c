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
};
state=STATE_CONNECTED;
turn=TEAM_WHITE;
return OK;
}
int isMatchSet(){}
int movePiece(coordinate_t src, coordinate_t dest){
if(state==STATE_PLAYING && turn==TEAM_WHITE){

}
return ERROR;
}

