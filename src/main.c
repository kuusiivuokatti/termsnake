#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>

#include "../include/main.h"

struct{
	int speed;
	int width;
	int height;
	int dWidth;
	int dHeight;
}game;

struct character{
	bool alive;
	int x;
	int y;
	int xPrev;
	int yPrev;
	int length;
	int direction;
	int score;
};

struct character *snake=NULL;
struct character enemy;

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
	snake[0].alive=true;
	snake[0].x=game.height/2;
	snake[0].y=game.width/2;
	snake[0].score=0;
	snake[0].length=1;
	enemy.alive=false;
}

void DrawGame(){
	printf(CLEAR_SCREEN);
	if(DEBUG){
		printf("px %d py %d ex %d ey %d s %d t0x %d t0y %d t1x %d t1y %d\n",snake[0].x,snake[0].y,enemy.x,enemy.y,snake[0].score,snake[0].x,snake[0].y,snake[1].x,snake[1].y);
	}else{
		printf("%s %s\nscore: %d\n",NAME,VERSION,snake[0].score);
	}	
	int i,j,k;
	bool tailFound=false;
	for(i=0;i<game.height;i++){
		for(j=0;j<game.width;j++){
			for(k=0;k<snake[0].length;k++){
				tailFound=false;
				if(snake[k].x==i && snake[k].y==j){
					if(k==0){
						printf("S");
					}else{
						printf("s");
					}
					tailFound=true;
					break;
				}
			}
			if(!tailFound){
				if(enemy.x==i && enemy.y==j){
					printf("E");
				}else if(i==0 || i==game.dHeight){
					printf("-");
				}else if(j==0 || j==game.dWidth){
					printf("|");
				}else{
					printf(".");
				}
			}
		}
		printf("\n");
	}
	if(snake[0].x==enemy.x && snake[0].y==enemy.y){
		enemy.alive=false;
		snake[0].score++;
		snake[0].length++;
	}
}

void UpdatePosition(){	
	snake[0].xPrev=snake[0].x;
	snake[0].yPrev=snake[0].y;
	switch(snake[0].direction){
		case 1:
			snake[0].x--;
			break;
		case 2:
			snake[0].x++;
			break;
		case 3:
			snake[0].y++;
			break;
		case 4:
			snake[0].y--;
			break;
	}
	if(snake[0].x<=0 || snake[0].x>=game.dHeight || snake[0].y<=0 || snake[0].y>=game.dWidth){
		snake[0].alive=false;
	}
	
	int i=0;
	for(i=1;i<snake[0].length;i++){
		snake[i].xPrev=snake[i].x;
		snake[i].yPrev=snake[i].y;
		snake[i].x=snake[i-1].xPrev;
		snake[i].y=snake[i-1].yPrev;
	}
	
	if(!enemy.alive){
		enemy.x=rand()%(game.dHeight-1)+1;
		enemy.y=rand()%(game.dWidth-1)+1;
		enemy.alive=true;
	}
}

void ReadInput(){
	int c;
	c=getchar();
	if(c==UP){
		if(snake[0].direction!=2){
			snake[0].direction=1;
		}
	}else if(c==DOWN){
		if(snake[0].direction!=1){
			snake[0].direction=2;
		}
	}else if(c==RIGHT){
		if(snake[0].direction!=4){
			snake[0].direction=3;
		}
	}else if(c==LEFT){
		if(snake[0].direction!=3){
			snake[0].direction=4;
		}
	}else if(c=='q'){
		snake[0].alive=false;
	}
}

int main(){
	struct winsize win;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
	game.height=(win.ws_row-2)/2;
	game.width=win.ws_col/2;
	game.dHeight=game.height-1;
	game.dWidth=game.width-1;
	
	SetTermFlag(0);
	
	snake=malloc(game.dHeight*game.dWidth*sizeof(*snake));
	
	ResetGame();
	DrawGame();

	srand(time(NULL));
	while(snake[0].alive){
		ReadInput();
		UpdatePosition();
		DrawGame();
		usleep(game.speed*10000);	
	}

	free(snake);

	SetTermFlag(1);

	return EXIT_SUCCESS;
}
