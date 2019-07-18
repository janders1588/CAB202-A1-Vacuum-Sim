#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <curses.h>
#include <cab202_graphics.h>

#define VAC_HEIGHT 10
#define VAC_WIDTH 16
#define SLIME_HEIGHT 5
#define SLIME_WIDTH 5
#define LITTER_HEIGHT 6
#define LITTER_WIDTH 12
#define DUST_HEIGHT 1
#define DUST_WIDTH 1
#define DOCK_HEIGHT 10
#define DOCK_WIDTH 10
#define GAME_OVER_HEIGHT 16
#define GAME_OVER_WIDTH 86
#define GOODBYE_HEIGHT 8
#define GOODBYE_WIDTH 69

#ifndef PROGRAM_IMAGES_H
#define PROGRAM_IMAGES_H

typedef enum {VACUUM, DUST, LITTER, SLIME, DOCK}IMAGE_NAME;

extern IMAGE_NAME image_names;

#endif // PROGRAM_IMAGES_H

char * vacuum_image();
char * dust_image();
char * litter_image();
char * slime_image();
char * dock_image();
char * game_over_image();
char * goodbye_image();
char * help_image();
void draw_image(double x,double y,char * bitmap,int width,int height,bool space_is_transparent);

