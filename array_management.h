#include <math.h>
#include <stdlib.h>
#include <cab202_graphics.h>
#include "program_images.h"

typedef struct coords
{
    double x,y;
    int name,pos;
}
/**
*   Structure for storing the data for images.

*   Members:
*       x: Double float value for x position.
*       y: double float value for y position.
*       name: Integer that represents name from enum IMAGE_NAME.
*       pos: Integer that represents the position in the image array.
*/
Coords;

/** Fills a structured array with random instantiated (x,y) coordinates
*   using check_space() to make sure that no image overlaps any previously
*   instantiated images.
*   int count - number of images to spawn
*   int width - width of image
*   int height - height of image
*   int image_name -- from the IMAGE_NAME enum {VACUUM=0, DUST=1,
*                                            LITTER=2, SLIME=3, DOCK=4}
*
*   Hint -- When using this method feed the largest object in first and then continue
*            down by image size, this will ensure the available space can be filled more
*            efficiently.
*/
void fill_arrays(int count,int width,int height,int image_name);


void add_single_image(int x,int y,int width,int height,int image_name);

/** Checks the input set of co-ordinates against all instantiated pixel
*   data to determine whether the image can be drawn in that space
*   double x,
*   double y,
*   int width,
*   int height,
*   int image_name -- from the IMAGE_NAME enum {VACUUM=0, DUST=1,
*                                             LITTER=2, SLIME=3, DOCK=4}
*/
bool check_space(double x,double y,int width,int height,int image_name);

/**
*   Returns the structured basic coordinates created through the fill array method
*   to be used for drawing or collision checks
*/
Coords * return_coords(void);

/**
*   Returns the structured pixel level coordinates created through the fill array method
*   to be used for drawing or collision checks
*/
Coords * return_pixel_array(void);

/**
*   Updates the pixel array with a modified array
*
*   Parameters:
*           new_pixel_data: A Coords struct contain modified pixel data to update the main
*
*/
void update_pixel_array(Coords * new_pixel_data);

/**
*   Clears all values from the structured pixel array and basic array
*/
void reset_pixel_array();

/**
*   Clears a single value from the structured basic array
*
*   Parameters:
*           pos: An integer representing the array position to clear
*/
void remove_from_basic_array(int pos);

/**
*   Integer representing the current dust images drawn in the screen space
*   calculated from the basic_coordinate_data structured array.
*/
int current_dust_count();

/**
*   Integer representing the current litter images drawn in the screen space
*   calculated from the basic_coordinate_data structured array.
*/
int current_litter_count();

/**
*   Integer representing the current slime images drawn in the screen space
*   calculated from the basic_coordinate_data structured array.
*/
int current_slime_count();
