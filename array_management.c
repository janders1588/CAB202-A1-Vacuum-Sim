#include <math.h>
#include <stdlib.h>
#include <cab202_graphics.h>
#include "program_images.h"


#define MAX_INSTANTIATED 1015
#define MAX_PIXEL_DATA 1800 //Calculated through (W*H)*Max of all objects
static int array_pos=0;
static int pix_array_pos=0;
IMAGE_NAME imagenames;

// See array_management.h for details
typedef struct coords
{
    double x,y;
    int name,pos;
}Coords;

// Setup Coords for the basic array and pixel array
static Coords basic_coordinate_data[MAX_INSTANTIATED];
static Coords pixel_coordinate_data[8000];


void add_to_pixel_array(double x[],double y[],int p_name, int arr_size)
{
    for(int i=0; i<arr_size; i++)
    {
        pixel_coordinate_data[pix_array_pos].name=p_name;
        pixel_coordinate_data[pix_array_pos].x=x[i];
        pixel_coordinate_data[pix_array_pos].y=y[i];
        pixel_coordinate_data[pix_array_pos].pos=array_pos;
        pix_array_pos++;
    }
}

// See array_management.h for details
bool check_space(double x,double y,int width,int height,int image_name)
{
    //Determine the return image based on image_name value
    char * bitmap = " ";
    if(image_name==VACUUM)
    {
        bitmap=vacuum_image();
    }
    if(image_name==DUST)
    {
        bitmap=dust_image();
    }
    if(image_name==LITTER)
    {
        bitmap=litter_image();
    }
    if(image_name==SLIME)
    {
        bitmap=slime_image();
    }
    if(image_name==DOCK)
    {
        bitmap=dock_image();
    }

    //create a temp object array
    double tmp_x[width*height];
    double tmp_y[width*height];
    int tmp_pos=0;

    //Draw the requested image into the temp array
    for(int j=0; j<=height-1; ++j)
    {
        for(int i=0; i<=width-1; ++i)
        {
            if(bitmap[i+j*width]!=' ')
            {
                tmp_x[tmp_pos] = x+i;
                tmp_y[tmp_pos] = y+j;
                tmp_pos++;
            }
        }
    }

    int overlap_count=0;

    /*  Cross check each pixel in the temp array with all currently
        instantiated pixels, for each pixel that intersects increase
        overlap count by 1  */
    for(int i=0; i<tmp_pos; i++)
    {
        for(int p=0; p<pix_array_pos; p++)
        {
            if(tmp_x[i]==pixel_coordinate_data[p].x&&tmp_y[i]==pixel_coordinate_data[p].y)
            {
                overlap_count++;
            }
        }
    }

    // If no pixels overlap then add the temp array to the pixel array and return true
    if(overlap_count==0)
    {
        add_to_pixel_array(tmp_x,tmp_y,image_name,width*height);
        return true;
    }
    // If any pixels overlap return false
    else if(overlap_count>0)
    {
        return false;
    }

    //Default return value
    return false;
}

// See array_management.h for details
void fill_arrays(int count,int width,int height,int image_name)
{
    // Get screen size for appropriate random values
    int screen_w,screen_h;
    get_screen_size(&screen_w,&screen_h);

    // Create a random point on screen for each requested count of an image type
    for(int s=0; s<count; ++s)
    {
        int tmp_x = rand()%(screen_w);
        int tmp_y = rand()%(screen_h);

        // If the value is within the simulation bounds then continue
        if((tmp_y > 4&& tmp_y+(height) < screen_h-2) &&
                (tmp_x >0&&tmp_x+(width+width/2)<screen_w))
        {
            /*  Use check_space function to determine if the image to be drawn will
                overlap an existing image, if return is true then the image is clear,
                has been added to the pixel array, and can have it origin point added
                to the basic array */
            if(check_space(tmp_x,tmp_y,width,height,image_name))
            {
                basic_coordinate_data[array_pos].x=tmp_x;
                basic_coordinate_data[array_pos].y=tmp_y;
                basic_coordinate_data[array_pos].name=image_name;
                basic_coordinate_data[array_pos].pos=array_pos;
                array_pos++;
            }
            // If image overlaps then retry the loop iteration again
            else
            {
                s--;
            }

        }
        // Try loop iteration again
        else
        {
            s--;
        }
    }
    /* Note:    This setup is not optimal as it will create a infinite loop when asked to
                instantiate more objects then can fit in the simulation space. */
}

/*  Modified fill_arrays function to be used in the do_operations of the main program
    created to give more control over where an image is placed rather than randomly generating
    a position */
void add_single_image(int x,int y,int width,int height,int image_name)
{
    int screen_w,screen_h;
    get_screen_size(&screen_w,&screen_h);
        if((y > 4&& y+(height) < screen_h-2) &&
                (x >0&&x+(width+width/2)<screen_w))
        {
                basic_coordinate_data[array_pos].x=x;
                basic_coordinate_data[array_pos].y=y;
                basic_coordinate_data[array_pos].name=image_name;
                basic_coordinate_data[array_pos].pos=array_pos;
                array_pos++;
        }
}

// See array_management.h for details
void update_pixel_array(Coords * new_pixel_data)
{
    /*  Iterate through each object in the pixel data array and replace
        with its new counterpart*/
    for(int i=0; i<pix_array_pos; i++)
    {
        pixel_coordinate_data[i].name=new_pixel_data[i].name,
        pixel_coordinate_data[i].pos=new_pixel_data[i].pos,
        pixel_coordinate_data[i].x=new_pixel_data[i].x,
        pixel_coordinate_data[i].y=new_pixel_data[i].y;
    }
}

// See array_management.h for details
Coords * return_coords(void)
{
    return basic_coordinate_data;
}

// See array_management.h for details
Coords * return_pixel_array(void)
{
    return pixel_coordinate_data;
}

/**
*   Clears a single value from the structured pixel array
*
*   Parameters:
*           pos: An integer representing the image to clear taken from
*                the basic array.
*/
static void remove_from_pixel_array(int image_pos)
{
    for(int d=0; d<pix_array_pos; d++)
    {
        if(pixel_coordinate_data[d].pos == image_pos)
        {
            pixel_coordinate_data[d].x = 0;
            pixel_coordinate_data[d].y=0;
            pixel_coordinate_data[d].name=0;
            pixel_coordinate_data[d].pos=0;
        }
    }
}

void remove_from_basic_array(int image_pos)
{
    /*  Iterate through the array until the need position is
        reached then set all values to zero*/
    for(int i=0; i<array_pos; i++)
    {
        if(image_pos==basic_coordinate_data[i].pos)
        {
            basic_coordinate_data[i].name=0,
            basic_coordinate_data[i].pos=0,
            basic_coordinate_data[i].x=0,
            basic_coordinate_data[i].y=0;
            remove_from_pixel_array(image_pos);
        }
    }

}

// See array_management.h for details
void reset_pixel_array()
{
    /*  Iterate through all values in each array
        and reset them to zero*/
    for(int i=0; i<pix_array_pos; i++)
    {
        pixel_coordinate_data[i].name=0,
        pixel_coordinate_data[i].pos=0,
        pixel_coordinate_data[i].x=0,
        pixel_coordinate_data[i].y=0;
    }
    for(int j=0; j<array_pos; j++)
    {
        basic_coordinate_data[j].name=0,
        basic_coordinate_data[j].pos=0,
        basic_coordinate_data[j].x=0,
        basic_coordinate_data[j].y=0;
    }
    array_pos=0;
    pix_array_pos=0;
}

// See array_management.h for details
int current_dust_count()
{
    int count=0;
    for(int i=0; i<MAX_INSTANTIATED; i++)
    {
        if(basic_coordinate_data[i].name==1)
        {
            count++;
        }
    }
    return count;
}

// See array_management.h for details
int current_litter_count()
{
    int count=0;
    for(int i=0; i<MAX_INSTANTIATED; i++)
    {
        if(basic_coordinate_data[i].name==2)
        {
            count++;
        }
    }
    return count;
}

// See array_management.h for details
int current_slime_count()
{
    int count=0;
    for(int i=0; i<MAX_INSTANTIATED; i++)
    {
        if(basic_coordinate_data[i].name==3)
        {
            count++;
        }
    }
    return count;
}
