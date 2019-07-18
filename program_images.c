#include <cab202_graphics.h>


char * vacuum_image()
{
    return
        "   @@@@@@@@@@   "
        "  @**********@  "
        " @************@ "
        "@**************@"
        "@**************@"
        "@**************@"
        "@**************@"
        " @************@ "
        "  @**********@  "
        "   @@@@@@@@@@   "
        ;
}

char* dock_image()
{
    return
        "##########"
        "##########"
        "##########"
        ;

}

char * dust_image()
{
    return ".";
}

char * litter_image()
{
    return
        "     &&     "
        "    &&&&    "
        "   &&&&&&   "
        "  &&&&&&&&  "
        " &&&&&&&&&& "
        "&&&&&&&&&&&&"
        ;
}
char * slime_image()
{
    return
        "  ~  "
        " ~~~ "
        "~~~~~"
        " ~~~ "
        "  ~  "
        ;
}

char * game_over_image()
{
    return
        "  _______  ___   __   __  __   __  ___      _______  _______  ___   _______  __    _  "
        " |       ||   | |  |_|  ||  | |  ||   |    |   _   ||       ||   | |       ||  |  | | "
        " |  _____||   | |       ||  | |  ||   |    |  |_|  ||_     _||   | |   _   ||   |_| | "
        " | |_____ |   | |       ||  |_|  ||   |    |       |  |   |  |   | |  | |  ||       | "
        " |_____  ||   | |       ||       ||   |___ |       |  |   |  |   | |  |_|  ||  _    | "
        "  _____| ||   | | ||_|| ||       ||       ||   _   |  |   |  |   | |       || | |   | "
        " |_______||___| |_|   |_||_______||_______||__| |__|  |___|  |___| |_______||_|  |__| "
        "                      _______  __   __  _______  ______    __                         "
        "                     |       ||  | |  ||       ||    _ |  |  |                        "
        "                     |   _   ||  |_|  ||    ___||   | ||  |  |                        "
        "                     |  | |  ||       ||   |___ |   |_||_ |  |                        "
        "                     |  |_|  ||       ||    ___||    __  ||__|                        "
        "                     |       | |     | |   |___ |   |  | | __                         "
        "                     |_______|  |___|  |_______||___|  |_||__|                        "
        "                      PRESS 'R' TO RESTART OR ANY KEY TO QUIT                         "
        "                                                                                      "    ;
}


char * goodbye_image()
{
    return
        "  _______  _______  _______  ______   _______  __   __  _______  __  "
        " |       ||       ||       ||      | |  _    ||  | |  ||       ||  | "
        " |    ___||   _   ||   _   ||  _    || |_|   ||  |_|  ||    ___||  | "
        " |   | __ |  | |  ||  | |  || | |   ||       ||       ||   |___ |  | "
        " |   ||  ||  |_|  ||  |_|  || |_|   ||  _   | |_     _||    ___||__| "
        " |   |_| ||       ||       ||       || |_|   |  |   |  |   |___  __  "
        " |_______||_______||_______||______| |_______|  |___|  |_______||__| "
        "                                                                     "
        ;
}

char * help_image()
{
    return
        "***********************************************************************************"
        "*            __   __  _______  _______  __   __  __   __  __   __                 *"
        "*           |  | |  ||   _   ||       ||  | |  ||  | |  ||  |_|  |                *"
        "*           |  |_|  ||  |_|  ||       ||  | |  ||  | |  ||       |                *"
        "*           |       ||       ||       ||  |_|  ||  |_|  ||       |                *"
        "*           |       ||       ||      _||       ||       ||       |                *"
        "*            |     | |   _   ||     |_ |       ||       || ||_|| |                *"
        "*             |___|  |__| |__||_______||_______||_______||_|   |_|                *"
        "*  _______  ___   __   __  __   __  ___      _______  _______  _______  ______    *"
        "* |       ||   | |  |_|  ||  | |  ||   |    |   _   ||       ||       ||    _ |   *"
        "* |  _____||   | |       ||  | |  ||   |    |  |_|  ||_     _||   _   ||   | ||   *"
        "* | |_____ |   | |       ||  |_|  ||   |    |       |  |   |  |  | |  ||   |_||_  *"
        "* |_____  ||   | |       ||       ||   |___ |       |  |   |  |  |_|  ||    __  | *"
        "*  _____| ||   | | ||_|| ||       ||       ||   _   |  |   |  |       ||   |  | | *"
        "* |_______||___| |_|   |_||_______||_______||__| |__|  |___|  |_______||___|  |_| *"
        "*                                                                                 *"
        "***********************************************************************************"
        "                                                                                   "
        "                     i,j,k,l: Move vacuum up, left, down, right                    "
        "                     b: Send vacuum to dock                                        "
        "                     d: Add one dust to room                                       "
        "                     s: Add one slime to room                                      "
        "                     t: Add one trash to room                                      "
        "                     m: Change the loop delay (milliseconds)                       "
        "                     o: Set simulation timeout (seconds)                           "
        "                     p: Toggle vacuum movement                                     "
        "                     r: Reset simulation                                           "
        "                     v: Move the vacuum to new x,y position                        "
        "                     y: Set battery level                                          "
        "                     ?: Show this screen                                           "
        "                                                                                   "
        "                     Press any key to return to the simulation!                    "
        ;
}

/**
*   Draw an image at a point in  screen space.
*
*   Parameters:
*           x,y: Two doubles that shows the x,y position of the upper left corner of the image.
*           bitmap: A char array that holds the pixel information for the function to draw.
*           width: An int that represents the pixel width of the image to be drawn.
*           height: An int that represents the pixel height of the image to be drawn.
*           space_is_transparent: A boolean that lets the function know whether the empty
                                    spaces in the image should be drawn as transparent.
*
*   Notes:
*       The height and width values for this function need to be the exact height and width
*   of the images including empty space otherwise the image will be drawn wrong or with random
*   characters from the buffer.
*/

void draw_image(double x,double y,char * bitmap,int width,int height,bool space_is_transparent)
{
    for(int j=0; j<=height-1; ++j)
    {
        for(int i=0; i<=width-1; ++i)
        {
            if(bitmap[i+j*width]!=' ')
            {
                draw_char(x+i,y+j,bitmap[i+j*width]);
            }
            else if (bitmap[i+j*width] ==' ' && !space_is_transparent)
            {
                draw_char(x+i,y+j,' ');
            }
        }
    }
}
