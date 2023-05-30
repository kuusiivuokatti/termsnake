#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "../include/debug.h"
#include "../include/main.h"
#include "../include/game.h"

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

int main(){	
	printf(CLEAR_SCREEN);
	SetTermFlag(0);
	SetupGame();
	SetTermFlag(1);
	return EXIT_SUCCESS;
}
