#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include "../include/main.h"

struct{
	int width;
	int height;
	int dWidth;
	int dHeight;
}window;

struct{
	bool over;
	int speed;
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
	bool alive;
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
	game.speed=7;
	player.x=window.dHeight/2;
	player.y=window.dWidth/2;
	player.score=0;
	player.length=1;
}

void DrawGame(){
	int i,j;
	printf(CLEAR_SCREEN);
	for(i=0;i<window.dHeight;i++){
		for(j=0;j<window.dWidth;j++){
			if(player.x==i && player.y==j){
				printf("S");
			}else if(enemy.x==i && enemy.y==j){
				printf("P");
			}else if(i==0 || i==window.dHeight-1){
				printf("-");
			}else if(j==0 || j==window.dWidth-1){
				printf("|");
			}else{
				printf(".");
			}
		}
		printf("\n");
	}
	if(player.x==enemy.x && player.y==enemy.y){
		enemy.alive=false;
		player.score++;
	}
}

void UpdatePosition(){
	switch(player.direction){
		case 1:
			player.x--;
			break;
		case 2:
			player.x++;
			break;
		case 3:
			player.y++;
			break;
		case 4:
			player.y--;
			break;
	}
	if(player.x<=0 || player.x>=window.dHeight-1 || player.y<=0 || player.y>=window.dWidth-1){
		game.over=true;
	}

	if(!enemy.alive){
		enemy.x=rand()%(window.dHeight-4)+4;
		enemy.y=rand()%(window.dWidth-4)+4;
		enemy.alive=true;
	}
}

void ReadInput(){
	int c;
	c=getchar();
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
	window.height=30;
	window.width=20;
	window.dHeight=window.height-2;
	window.dWidth=window.width-2;
	ResetGame();
	DrawGame();

	srand(time(NULL));
	while(!game.over){
		ReadInput();
		UpdatePosition();
		DrawGame();
		usleep(game.speed*10000);	
	}
	SetTermFlag(1);

	return EXIT_SUCCESS;

}
