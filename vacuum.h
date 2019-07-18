#include <math.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <curses.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "vector_math.h"
#include "array_management.h"
#include "program_images.h"

void set_dock_coordinates(double x,double y);

//***************************Movement************************************
/**
*   Return the current vacuum heading
*/
int get_heading();

/**
*   Return the current vacuum x position
*/
int get_vac_x();

/**
*   Return the current vacuum y position
*/
int get_vac_y();

/**
*   Return the vacuums current movement state
*/
bool get_movement_state();

/**
*   Sets the vacuum at specific x,y position
*
*   Parameters:
*           x,y: Two integers that represent the screen space position to move the
*                vacuum to.
*/
void set_vacuum_coordinates(int x, int y);

/**
*   Offsets the vacuum at x,y values
*
*   Parameters:
*           x,y: Two integers that represent the value to offset the vacuum
*/
void change_vacuum_coordinates(int x, int y);

/**
*   Draw the vacuum at its predefined x,y location.
*
*   Notes: This function gets it image from the program_images.h
*/
void draw_vacuum();

/**
*   Function to update the vacuum position and check the vacuum state.
*/
void update_vacuum();

/**
*   Function to set the vacuums current heading
*
*   Parameters:
*           heading: An integer value that represents the direction of the vacuum to
*                    move in degrees.
*
*   Notes: The orientation of the screen space is as follows
*          0-Right, 90-Down, 180-Left, 270-Up.
*/
void set_heading(int heading_in);

/**
*   Function to modify the vacuums movement state
*
*   Parameters:
*           is_moving: A boolean value to state whether the vacuum should be moving.
*/
void set_movement_state(bool is_moving);

/**
*   Function to modify the vacuums movement state
*
*   Parameters:
*           is_docking: A boolean value to state whether the vacuum is docking
*/
void set_docking_state(bool is_docking);

/**
*   Function to sent the vacuum to a new position with a new heading.
*
*   Parameters:
*           x: Integer that represents new x coordinate
*           y: Integer that represents new y coordinate
*           heading: Integer that represents new heading in degrees
*/
void manual_reposition_vacuum (int x,int y,int direction);

/**
*   Function to send the vacuum to the dock (for debugging)
*/
void dock_vacuum();
//**********************************************************************

//***************************Battery************************************
/**
*   Returns current battery charge
*/
int get_battery_charge();

/**
*   Returns current load weight
*/
int get_load_weight();

/**
*   Sets a tick value at +1 per time slice to be used by drain_battery() in vacuum.c
*/
void drain_battery_tick();

/**
*   Set the battery level
*
*   Parameters:
*           charge: An integer that represents battery charge form 0-100
*/
void set_battery_level(int charge);
//*************************************************************************

//***************************Load************************************
/**
*   Function to add trash weight to load_weight variable
*
*   Parameters:
*       image_name: The enum value associated with the name in IMAGE_NAMES.
*/
void add_weight(int image_name);

/**
*   Function to set the vacuum load weight
*
*   Parameters:
*           weight: An integer that represents the weight in grams to change to.
*/
void set_load_weight(int weight);

//******************************Game State******************************
/**
*   Returns current game_over state
*/
bool get_game_state();

/**
*   Set the game state
*
*   Parameters:
*           is_game_over: Boolean to trigger game over
*/
void set_game_state(bool is_game_over);
//***********************************************************************

void update_screen_size(int width, int height);

