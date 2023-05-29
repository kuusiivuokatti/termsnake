#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>

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

void DrawGame(){
	int i,j,k;
	bool tailFound=false;
	for(i=0;i<game.height;i++){
		for(j=0;j<game.width;j++){
			for(k=0;k<=snake[0].length;k++){
				tailFound=false;
				if(snake[k].x==i && snake[k].y==j){
					if(k>0){
						printf("s");
					}else{
						printf("S");
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
					printf("%d",i);
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

void ResetGame(){
	game.speed=7;
	snake[0].alive=true;
	snake[0].x=game.height/2;
	snake[0].y=game.width/2;
	snake[0].score=0;
	snake[0].length=5;
	enemy.alive=false;
}

int main(){
	struct winsize win;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
	game.height=(win.ws_row-2)/2;
	game.width=win.ws_col/2;
	game.dHeight=game.height-1;
	game.dWidth=game.width-1;

	snake=malloc(5*sizeof(*snake));
	ResetGame();
	snake[0].x=10;
	snake[0].y=10;

	snake[1].x=10;
	snake[0].y=10;
	
	for(int i=0;i<=10;i++){
		snake[0].xPrev=snake[0].x;
		snake[0].yPrev=snake[0].y;
		if(i<=5){
			snake[0].x--;
		}else{
			snake[0].y--;
		}	
		for(int j=1;j<=snake[0].length;j++){
			snake[j].xPrev=snake[j].x;
			snake[j].yPrev=snake[j].y;
			snake[j].x=snake[j-1].xPrev;
			snake[j].y=snake[j-1].yPrev;
			printf("%d x=%d, y=%d\n",j,snake[j].x,snake[j].y);
		}
		DrawGame();
	}
	free(snake);

	return EXIT_SUCCESS;
}
