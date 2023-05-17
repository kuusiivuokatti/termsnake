#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <termios.h>
#include <fcntl.h>

#include "../include/main.h"

struct{
	bool over;
	int rnd;
	int width;
	int height;
}game;


struct{
	int x;
	int y;
	int length;
	int direction;
	int score;
}player;

struct{
	int x;
	int y;
}enemy;

void SetTermFlag(int mode){
// TODO : consider changing to something more universal
	struct termios term;
	tcgetattr(0,&term);
	if(mode==0){
		fcntl(0,F_SETFL,O_NONBLOCK);
		term.c_lflag&=~ICANON;

		if(!DEBUG){term.c_lflag&=~ECHO;}

		term.c_cc[VMIN]=1;
		term.c_cc[VTIME]=0;
		tcsetattr(0,TCSANOW,&term);
	}else{
		int flag=fcntl(0,F_GETFL);
		flag&=~O_NONBLOCK;
		fcntl(0,F_SETFL,flag);
		term.c_lflag|=ICANON;

		if(!DEBUG){term.c_lflag|=ECHO;}

		tcsetattr(0,TCSANOW,&term);
	}
}

void ResetGame(){
	game.rnd=0;
	game.height=30;
	game.width=20;
	player.x=5;
	player.y=10;
	player.score=0;
	player.length=1;
}

void DrawGame(){
	int i,j;
	printf(CLEAR_SCREEN);
	if(DEBUG){printf("round: %d ",game.rnd);}
	printf("score: %d\n",player.score);
	for(i=0;i<game.height;i++){
		for(j=0;j<game.width;j++){
			if(player.x==i && player.y==j){
				printf("¤");
			}else if(i==0 || i==game.height-1){
				printf("-");
			}else if(j==0 || j==game.width-1){
				printf("|");
			}else{
				printf(".");
			}
		}
		printf("\n");
	}
	game.rnd++;
}

void UpdatePosition(){
	switch(player.direction){
		case 1:
			player.y--;
			break;
		case 2:
			player.y++;
			break;
		case 3:
			player.x++;
			break;
		case 4:
			player.x--;
			break;
	}
	if(DEBUG){printf("player x is %d, y is %d\n",player.x,player.y);}
}

void ReadInput(){
	int c;
	c=getchar();

	printf("here");
	if(DEBUG){printf("c equals %d",c);sleep(1);}
	
	if(c==UP){
		player.direction=1;
	}else if(c==DOWN){
		player.direction=2;
	}else if(c==RIGHT){
		player.direction=3;
	}else if(c==LEFT){
		player.direction=4;
	}else if(c=='q'){
		game.over=true;
	}
}

int main(){
	SetTermFlag(0);
	ResetGame();
	while(!game.over){
		ReadInput();
		UpdatePosition();
		if(player.x<=0 || player.x>=game.height || player.y<=0 || player.y>=game.width){
			game.over=true;
		}else{
			DrawGame();
		}
		sleep(1);
	}	
	SetTermFlag(1);
	
	if(DEBUG){int c;c=getchar();}

	return 0;
}
