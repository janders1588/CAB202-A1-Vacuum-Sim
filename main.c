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
#include "program_images.h"
#include "vacuum.h"

// Global Variables
int delay = 30;
int timeout2 = INT_MAX;
int screen_h, screen_w;
IMAGE_NAME imagenames;

//Timer variables
double start_time;
double timer;
double temp=0;


//int count;
//Game State
bool paused = false;
bool quit = false;
bool timed_out()
{
    return timer >= timeout2;

}

//Dock state
double dx, dy;

//Rubbish state
#define MAX_DUST 1000
#define MAX_SLIME 10
#define MAX_LITTER 5

int dust_count =0;
int slime_count=0;
int litter_count=0;

void game_timer()
{
    timer = (get_current_time() - start_time)+temp;
}

/**
*   Draws appropriate information in the status area
*/
void status_display(int w,int h)
{
    char * name_label = "n10318399";
    char elapsed_time[40];
    sprintf(elapsed_time,"Elapsed: %02i:%02i",(int)timer/60,(int)timer%60);
    char heading_label[40];
    double heading_int = fmod(get_heading(),360); //Get heading as modulo 360
    if(heading_int<0)
    {
        heading_int+=360.0;
    }
    sprintf(heading_label,"Heading: %i",(int)heading_int);
    char load_weight_label[40];
    sprintf(load_weight_label,"Load Weight(g): %i",get_load_weight());
    char battery_label[40];
    sprintf(battery_label,"Battery: %i%%",get_battery_charge());
    char litter_label[40];
    sprintf(litter_label,"Litter (d,s,l):%i,%i,%i",current_dust_count(),current_slime_count(),current_litter_count());

    draw_string((w/4)/2,1,name_label);
    draw_string((w/4)/2,3,elapsed_time);
    draw_string((w/2)-(sizeof(heading_label)/4),1,heading_label);
    draw_string((w/2)-(sizeof(load_weight_label)/4),3,load_weight_label);
    draw_string(w-(w/4)+(sizeof(battery_label)/8),1,battery_label);
    draw_string(w-(w/4)+(sizeof(litter_label)/8),3,litter_label);
}

/**
*   Draws the status area at the top portion of the screen using '-',
*   '|', and '+' symbols for horizontal, vertical and corners respectively.
*/
void draw_status_area(int w,int h)
{
    int xpos = 0;

    for(int i=3; i>=0; --i)
    {
        xpos = ((w/3)*i); //column division point
        //Draw in the horizontals at line 0 and 2 skipping '+' char from the verticals
        switch(i)
        {
        case 0:
            draw_line(1,0,(xpos+(w/3*(i+1))-1),0,'-');//Top row column1
            draw_line(1,2,(xpos+(w/3*(i+1))-1),2,'-');//Middle row column1
        case 1:
            draw_line(xpos+1,0,(xpos+(w/3*(i))-1),0,'-');//Top row column2
            draw_line(xpos+1,2,(xpos+(w/3*(i))-1),2,'-');//Middle row column2
        case 2:
            draw_line(xpos+1,0,(xpos+(w/3*(i-1))),0,'-');//Top row column 3
            draw_line(xpos+1,2,(xpos+(w/3*(i-1))),2,'-');//Middle row column3
        }
        //Draw in the verticals at start, 1/2, 2/3
        if(i!=3)
        {
            draw_char(0+xpos,0,'+');
            draw_char(0+xpos,1,'|');
            draw_char(0+xpos,2,'+');
            draw_char(0+xpos,3,'|');
        }
        else
        {
            //Far right line needs to be width-1
            draw_char(w-1,0,'+');
            draw_char(w-1,1,'|');
            draw_char(w-1,2,'+');
            draw_char(w-1,3,'|');
        }
    }
    status_display(w,h);
}

/**
*   Draws border using '-', '|', and '+' symbols for horizontal,
*   vertical and corners respectively.
*/
void draw_border()
{
    fit_screen_to_window();
    get_screen_size(&screen_w, &screen_h);
    update_screen_size(screen_w,screen_h);
    const int chh = '|';
    const int chw = '-';
    const int chb = '+';

    draw_char(0,4,chb); // Top left
    draw_line(0, 5, 0, screen_h - 4, chh); // Top left - bottom left
    draw_char(screen_w-1,4,chb); // Top right
    draw_line(1, 4, screen_w - 2, 4, chw); // Top
    draw_char(0,screen_h-3,chb); // bottom left
    draw_line(1, screen_h - 3, screen_w - 2, screen_h - 3, chw); // Bottom
    draw_char(screen_w-1,screen_h-3,chb); // Bottom right
    draw_line(screen_w - 1, 5, screen_w - 1, screen_h - 4, chh); // Top right - bottom right
    draw_status_area(screen_w,screen_h);
}

/**
*   Draw the dock at the top center of the screen under the status area
*/
void draw_dock()
{
    draw_image(dx,
               dy,
               dock_image(),
               10,
               3,
               true);
}

/**
*   Draw all instantiated object in the simulation space
*/
void draw_all_pixels()
{
    Coords * pixels = return_pixel_array();
    int array_size = MAX_DUST+
                     (MAX_LITTER*(LITTER_HEIGHT*LITTER_WIDTH))+
                     (MAX_SLIME*(SLIME_HEIGHT*SLIME_WIDTH));
    for(int i=0; i<array_size; i++)
    {
        if(pixels[i].name==1)
        {
            draw_char(pixels[i].x,pixels[i].y,'.');
        }
        if(pixels[i].name==2)
        {
            draw_char(pixels[i].x,pixels[i].y,'&');
        }
        if(pixels[i].name==3)
        {
            draw_char(pixels[i].x,pixels[i].y,'~');
        }
    }
}

/**
*   Call all draw functions
*/
void draw_all()
{
    game_timer(false);
    clear_screen();
    draw_border();
    draw_dock();
    draw_all_pixels();
    draw_vacuum();
    show_screen();
}

/**
*   Ask user for trash count input
*/
void get_trash_input()
{
    show_screen();
    start_time = get_current_time();
    game_timer(true);

    litter_count = get_int("How much rubbish do yo want?");
    if(litter_count>MAX_LITTER)
    {
        litter_count=MAX_LITTER;
    }
    //stored_litter_count=litter_count;

    slime_count = get_int("How much slime do yo want?");
    if(slime_count>MAX_SLIME)
    {
        slime_count=MAX_SLIME;
    }
    //stored_slime_count=slime_count;

    dust_count = get_int("How much dust do yo want?");
    if(dust_count>MAX_DUST)
    {
        dust_count = MAX_DUST;
    }
    //stored_dust_count = dust_count;
}

/**
*   Create an array of coordinates for the trash using the fill_arrays() function in array_management.h
*   using the count values from the get_trash_input() and image data to ensure that no object overlaps
*   anything else on the screen.
*
*   Notes:
*       The fill_array() functions should be called using the largest image first and working down
*   to the smallest image, this makes it easier for the function to find available empty space.
*/
void setup_trash_array()
{
    fill_arrays(litter_count,LITTER_WIDTH,LITTER_HEIGHT,LITTER);
    fill_arrays(slime_count,SLIME_WIDTH,SLIME_HEIGHT,SLIME);
    fill_arrays(dust_count,DUST_WIDTH,DUST_HEIGHT,DUST);
}

/**
*   Setup the initial x,y position for the vacuum.
*/
void setup_vac()
{
    set_vacuum_coordinates(screen_w/2 - VAC_WIDTH/2,
                           screen_h/2 - VAC_HEIGHT/2);

    dx = screen_w/2-(DOCK_WIDTH/2);
    dy = 5;
    set_dock_coordinates(dx,dy);
    check_space(dx,dy,10,3,DOCK);
    check_space(get_vac_x(),get_vac_y(),VAC_WIDTH,VAC_HEIGHT,VACUUM);
}

/**
*   Call all setup functions.
*/
void setup ()
{
    start_time = get_current_time();
    draw_border();
    setup_vac();
    get_trash_input();
    setup_trash_array();
    draw_all();
}

/**
*   Pause vacuum
*/
void do_paused()
{
    paused = !paused;

}

/**
*   Set timeout value in seconds
*/
void do_timeout()
{
    timeout2=get_int("time out");
}

/**
*   Set the battery charge value
*/
void do_battery_cheat()
{
    set_battery_level(get_int("Set battery charge:"));
}

void help_screen()
{
    clear_screen();
    paused = true;
    set_movement_state(false);
    draw_image(screen_w/2-41,0,help_image(),83,32,false);
    show_screen();
    while (get_char()>0) {}
    while (!timed_out()&&wait_char()<=0) {}
    paused=false;
    set_movement_state(true);
    draw_all();
}

/**
*   Function to reset the game state
*/
void reset()
{
    clear_screen();
    set_movement_state(false);
    start_time = get_current_time();
    reset_pixel_array();
    timer = 0;
    set_heading(90);
    set_battery_level(100);
    set_load_weight(0);
    setup();
}

/**
*   Check if x value is within simulation space and alter
*   if not. Returns modified or original value.
*
*   Parameters:
*           x: Integer representing X coordinate value
*           width: Integer representing the width of the image
*                  being tested
*/
double check_screen_space_x(int x,int width)
{
    if(x<1)
    {
        return 1;
    }
    else if(x+width > screen_w-1)
    {
        return screen_w-(width+1);
    }
    else{return x;}
}

/**
*   Check if y value is within simulation space and alter
*   if not. Returns modified or original value.
*
*   Parameters:
*           y: Integer representing Y coordinate value
*           height: Integer representing the height of the image
*                   being tested
*/
double check_screen_space_y(int y,int height)
{
    if(y<5)
    {
        return 5;
    }
    else if(y+height > screen_h-3)
    {
        return screen_h-(height+3);
    }
    else{return y;}
}

/**
*   Operation to drop a trash type at defined x,y position
*
*   Parameters:
*           name: Integer representing name from IMGE_NAME enum
*/
void do_trash_drop(int name)
{
    int x = get_int("Input x coordinate");
    int y = get_int("Input y coordinate");
    char * occupied = "  Space is occupied  \n"
                      "                       ";
    if(name == DUST)
    {
        if(dust_count<MAX_DUST)
        {
            x = check_screen_space_x(x, DUST_WIDTH);
            y = check_screen_space_y(y, DUST_HEIGHT);

            if(check_space(x,y,DUST_WIDTH,DUST_HEIGHT,DUST))
            {
                add_single_image(x,y,DUST_WIDTH,DUST_HEIGHT,DUST);
            }
            else
            {
                draw_formatted_lines(screen_w/2-10,screen_h-2,occupied);
                show_screen();
                sleep(1);
            }
        }
    }
    if(name == LITTER)
    {
        if(litter_count<MAX_LITTER)
        {
            x= check_screen_space_x(x, LITTER_WIDTH);
            y = check_screen_space_y(y, LITTER_HEIGHT);

            if(check_space(x,y,LITTER_WIDTH,LITTER_HEIGHT,LITTER))
            {
                add_single_image(x,y,LITTER_WIDTH,LITTER_HEIGHT,LITTER);
            }
            else
            {
                draw_formatted_lines(screen_w/2-10,screen_h-2,occupied);
                show_screen();
                sleep(1);
            }
        }
    }
    if(name == SLIME)
    {
        if(slime_count<MAX_SLIME)
        {
            x= check_screen_space_x(x, SLIME_WIDTH);
            y = check_screen_space_y(y, SLIME_HEIGHT);

            if(check_space(x,y,SLIME_WIDTH,SLIME_HEIGHT,SLIME))
            {
                add_single_image(x,y,SLIME_WIDTH,SLIME_HEIGHT,SLIME);
            }
            else
            {
                draw_formatted_lines(screen_w/2-10,screen_h-2,occupied);
                show_screen();
                sleep(1);
            }
        }
    }
}

void do_operations(int ch)
{
    if(ch=='b'||ch == 'B')
    {
        set_docking_state(true);
    }
    if(ch=='d'||ch == 'D')
    {
        do_trash_drop(DUST);
    }
    if(ch=='m'||ch == 'M')
    {
        int tmp = get_int("Set delay: ");
        if(tmp < 10)
        {
            delay=10;
        }
        else
        {
            delay = tmp;
        }
    }
    if(ch=='o'||ch == 'O')
    {
        timeout2 = get_int("Set timeout: ");
    }
    if(ch=='p'||ch == 'P')
    {
        set_movement_state(!get_movement_state());
        set_heading(get_heading());
    }
    if(ch=='r'||ch == 'R')
    {
        reset();
    }
    if(ch=='s'||ch == 'S')
    {
        do_trash_drop(SLIME);
    }
    if(ch=='t'||ch == 'T')
    {
        do_trash_drop(LITTER);
    }
    if(ch=='v'||ch == 'V')
    {
        int x = get_int("New x coordinate: ");
        int y = get_int("New y coordinate: ");
        int direction = get_int("New heading: ");
        manual_reposition_vacuum (x,y,direction);
    }
    if(ch=='y'||ch == 'Y')
    {
        do_battery_cheat();
    }
    if(ch=='w'||ch == 'W')
    {
        int load = get_int("New load value: ");
        set_load_weight(load);
    }
    if(ch=='i'||ch == 'I')
    {
        change_vacuum_coordinates(0,-1);
    }
    if(ch=='j'||ch == 'J')
    {
        change_vacuum_coordinates(-1,0);
    }
    if(ch=='k'||ch == 'K')
    {
        change_vacuum_coordinates(0,1);
    }

    if(ch=='l'||ch == 'L')
    {
        change_vacuum_coordinates(1,0);
    }
    if(ch=='h'||ch=='H')
    {
        int head = get_int("Direction (degrees): ");

        set_heading(head);
    }
    if(ch==47 || ch=='?')
    {
        help_screen();
    }
}

void loop()
{
    int ch = get_char();
    if(ch=='q'||ch=='Q')
    {
        quit = true;
        return;
    }
    else if (ch >= ' ')
    {
        do_operations(ch);
    }
    else if(!paused)
    {
        draw_all();
        drain_battery_tick();
        update_vacuum();
    }
}

/**
*   Display Simulation Over over current screen
*/
void sim_over_screen()
{
    draw_image(screen_w/2-(GAME_OVER_WIDTH/2),
               screen_h/2-(GAME_OVER_HEIGHT/2),
               game_over_image(),
               GAME_OVER_WIDTH,
               GAME_OVER_HEIGHT,
               false);
    show_screen();
    char in = wait_char();
    if(in=='r'||in == 'R')
    {
        set_game_state(false);
        reset();
    }
}

/**
*   Display goodbye screen over current screen
*/
void goodbye_screen()
{
    draw_image(screen_w/2-(GOODBYE_WIDTH/2),
               screen_h/2-(GOODBYE_HEIGHT/2),
               goodbye_image(),
               GOODBYE_WIDTH,
               GOODBYE_HEIGHT,
               false);
}

int main()
{
    srand(get_current_time());
    setup_screen();
    setup();

    while (!get_game_state()&&!timed_out()&&!quit)
    {
        loop();
        timer_pause( delay );
        if(get_game_state()|| timed_out())
        {
            sim_over_screen();
            clear_screen();
        }
    }
    if(quit)
    {
        goodbye_screen();
        show_screen();
        get_int("Press Any Key...");
    }

    return 0;
}
