#ifndef MAIN_H
#define MAIN_H

#include "version.h"
#include "debug.h"

#define CLEAR_SCREEN "\e[1;1H\e[2J"

// TODO : unlikely to be reached, but consider changing to something based on something
#define MAX_SCORE 1000000000

// TODO : consider changing to something more universal
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

int main();

#endif
