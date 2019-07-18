#include <cab202_timers.h>
#include "vector_math.h"
#include "array_management.h"
#include "program_images.h"
#include "helpers.h"
#include "main.h"

#define DUST_WEIGHT 1;
#define SLIME_WEIGHT 5;
#define LITTER_WEIGHT 20;
#define VAC_SPEED 0.3;

static double battery_drain;

//Vacuum state
static double vx, vy, vdx, vdy;
static int heading =90;
static bool updating_vac=false;
static bool docking=false;
static bool docked = false;
static bool charging=false;
static bool vac_full=false;
static bool vac_moving;
static int load_weight=0;
static int battery_charge=100;

static double dx,dy;
static int screen_w,screen_h;

IMAGE_NAME imagenames;

//Game state
static bool game_over = false;

//******************** Return Variables********************
int get_battery_charge()
{
    return battery_charge;
}
int get_load_weight()
{
    return load_weight;
}
int get_heading()
{
    return heading;
}
int get_vac_x()
{
    return vx;
}
int get_vac_y()
{
    return vy;
}

bool get_game_state()
{
    return game_over;
}

bool get_movement_state()
{
    return vac_moving;
}

/**
*   Checks that the new coordinates do not exceed screen space and alters coordinates
*   if they do.
*/
static bool check_before_change(int x, int y)
{
    if(vx+x < 1)
    {
        vx=1;
        return true;

    }
    else if((vx+x)+VAC_WIDTH > screen_w-1)
    {
        vx=screen_w-(VAC_WIDTH+1);
        return true;
    }
    else if(vy+y < 5)
    {
        vy=5;
        return true;

    }
    else if((vy+y)+VAC_HEIGHT > screen_h-3)
    {
        vy=screen_h-(VAC_HEIGHT+3);
        return true;

    }
    else
    {
        return false;
    }
}
//*******************************************************


void set_game_state(bool is_game_over)
{
    game_over = is_game_over;
}

void change_vacuum_coordinates(int x, int y)
{
    if(check_before_change(x,y)) {}
    else
    {
        vx+=x;
        vy+=y;
    }
}

void set_vacuum_coordinates(int x, int y)
{
        vx=x;
        vy=y;
}

void set_movement_state(bool is_moving)
{
    vac_moving=is_moving;
}

void set_dock_coordinates(double x,double y)
{
    dx=x;
    dy=y;
}

void set_docking_state(bool is_docking)
{
    docking = is_docking;
}

void draw_vacuum()
{
    char * bitmap = vacuum_image();

    for(int j=0; j<=VAC_HEIGHT-1; ++j)
    {
        for(int i=0; i<=VAC_WIDTH-1; ++i)
        {
            if(bitmap[i+j*VAC_WIDTH]!=' ')
            {
                draw_char(vx+i,vy+j,bitmap[i+j*VAC_WIDTH]);
            }
        }
    }
}

/**
*   Send the vacuum in the direction of the set heading
*/
static void go_to()
{
    if(vac_moving)
    {
        vx+=vdx;
        vy+=vdy;
    }
}

/**
*   Set the heading of the vacuum in degrees
*
*   Parameters:
        heading_in: An integer that represents the direction for the vacuum to face in degrees
*/
void set_heading(int heading_in)
{
    const double step = VAC_SPEED;
    heading = heading_in;
    vdy = step * sin(degree_to_radian(heading));
    vdx = step * cos(degree_to_radian(heading));
}

void drain_battery_tick()
{
    battery_drain++;
}

/*
*   Set the battery to drain while vacuum is moving at a rate of 1% per
*   40 time slices which at vacuum speed 0.3 is approximately 3 units per 1%
*/
static void drain_battery()
{
    if(vac_moving)
    {
        int tmp = battery_drain/40;
        if(tmp >= 1)
        {
            battery_drain=0;
            battery_charge--;
        }
    }
}

/**
*   Function to charge the battery from current charge to 100%
*/
static void charge_battery()
{
    int tmp = 100 - battery_charge;
    for(int i=1; i<=tmp; i++)
    {

        battery_charge++;
        usleep(42*1000);
        clear_screen();
        draw_all();
        draw_formatted_lines(screen_w/2-3,screen_h-2,"DOCKED");
        show_screen();
    }
}

void check_docking_state()
{
    if(docked)
    {
        draw_formatted_lines(screen_w/2-3,screen_h-2,"DOCKED");
    }

    if(docking)
    {
        set_heading(target_object(screen_h,screen_w,vx,vy,dx-3,dy+3));
    }
    if(round(vy) == round(dy+3) && round(vx)==round(dx-3) )
    {
        docked = true;
        vdx=vdy=0;
        load_weight=0;
        //charging = true;
        //charge_battery();
        //charging = false;
        docking=false;
        set_heading(target_object(screen_h,screen_w,vx,vy,rand()%screen_w,rand()%screen_h));
        docked = false;
    }
}

/**
*   Function to check the charge of the battery when called and send
*   the dock to recharge if the charge is below the threshold
*/
static void check_battery_state()
{
    if((battery_charge <=25 && charging==false) || vac_full)
    {
        drain_battery();
        updating_vac = false;
        go_to();
        if(vy+VAC_HEIGHT/2<screen_h/2)
        {
            if(!docking)
            {
                set_heading(target_object(screen_h,screen_w,vx,vy,dx-3,screen_h/2-10));
            }
            if(round(vx)==round(dx-3)&&round(vy)==round(screen_h/2-10)&&!docking)
            {
                docking = true;
            }
        }
        else
        {
            docking=true;
        }

        if(docking)
        {
            set_heading(target_object(screen_h,screen_w,vx,vy,dx-3,dy+3));
        }
        if(round(vy) == round(dy+3) && round(vx)==round(dx-3) )
        {
            charging = true;
            docked = true;
            vdx=vdy=0;
            if(!vac_full)
            {
                charge_battery();
            }
            if(vac_full)
            {
                load_weight=0;
            }
            docking = false;
            charging = false;
            docked = false;
            set_heading(target_object(screen_h,screen_w,vx,vy,rand()%screen_w,rand()%screen_h));
        }
    }
    else
    {
        updating_vac = true;
    }

    if(battery_charge <= 0)
    {
        battery_charge=0;
        game_over=true;
    }
}

/**
*   Function to set the vacuum battery level
*/
void set_battery_level(int charge)
{
    if(charge<0)
    {
        battery_charge=0;
    }
    else if(charge>100)
    {
        battery_charge=100;
    }
    else
    {
        battery_charge=charge;
    }
}

/**
*   Function to add trash weight to load_weight variable
*
*   Parameters:
*       image_name: The enum value associated with the name in IMAGE_NAMES.
*/
void add_weight(int image_name)
{
    if(image_name==DUST)
    {
        load_weight += DUST_WEIGHT;
    }
    if(image_name==LITTER)
    {
        load_weight+=  LITTER_WEIGHT;
    }
    if(image_name==SLIME)
    {
        load_weight += SLIME_WEIGHT;
    }
}

/**
*   Function to set the vacuum load weight
*/
void set_load_weight(int weight)
{
    if(weight < 0)
    {
        weight = 0;
    }
    load_weight= weight;
}

/**
*   Function to check the load weight each slice and return to base if
*   greater than the threshold.
*/
static void check_vac_load()
{
    if(load_weight>65)
    {
        set_load_weight(65);
    }

    if(load_weight>45)
    {
        vac_full=true;
    }
    else
    {
        vac_full=false;
    }
}


/**
*   Function to check the vacuum image against all trash instantiated in the
*   simulation space and remove the image if a pixel collides with the vacuum
*/
static void check_collision()
{
    //Take a copy of the current pixel array at the time of checking
    Coords * tmp_pixel_array = return_pixel_array();
    int array_size = 1800;
    char * bitmap = vacuum_image();

    //Setup temp array at the size of hight * width of vacuum image
    double tmp_x[10*16];
    double tmp_y[10*16];
    int tmp_pos=0;

    //Draw the vacuum into the temp array
    for(int j=0; j<=VAC_HEIGHT-1; ++j)
    {
        for(int i=0; i<=VAC_WIDTH-1; ++i)
        {
            if(bitmap[i+j*VAC_WIDTH]!=' ')
            {
                tmp_x[tmp_pos] = vx+i;
                tmp_y[tmp_pos] = vy+j;
                tmp_pos++;
            }
        }
    }
    int image_name=0;
    /*Check the position of every pixel in the vacuum against the
        pixels of every collectible image in the room*/
    for(int t=0; t<tmp_pos; t++)
    {
        for(int p=0; p<array_size; p++)
        {
            if(round(tmp_x[t]) == round(tmp_pixel_array[p].x) &&
                    round(tmp_y[t]) == (tmp_pixel_array[p].y))
            {
                if(tmp_pixel_array[p].name==DUST||
                        tmp_pixel_array[p].name==LITTER||
                        tmp_pixel_array[p].name==SLIME)
                {
                    /* If pixels overlap then get the name of the image
                        and it's position in the basic array and remove it
                        from the basic image array*/
                    int image_pos=tmp_pixel_array[p].pos;
                    image_name = tmp_pixel_array[p].name;
                    remove_from_basic_array(image_pos);
                }
            }
        }
    }
    /* Use the name gathered from the overlapped pixel to determine what  weight to add
        to current load (initialized value of 0 will be ignored) by the add weight function)*/
    add_weight(image_name);
}

// Update the vacuum and check its state
void update_vacuum()
{
    //Set temp variables
    double new_x = round(vx+vdx);
    double new_y = round(vy+vdx) - VAC_HEIGHT/2;
    double new_head;

    //Check vacuums current state
    check_vac_load();
    check_battery_state();
    check_docking_state();

    if(!vac_full)
    {
        check_collision();
    }

    //Update the vacuum position
    if(updating_vac)
    {
        srand(get_current_time());
        //Check that next x position is within screen bounds
        //If not stop vacuum and change heading
        if(new_x == 1||new_x + VAC_WIDTH == (screen_w))
        {
            vdx=vdy=0;
            if(new_x ==1)
            {
                if(vac_moving)
                {
                    new_head = target_object(screen_h,screen_w,vx,vy,rand()%screen_w,rand()%screen_h);
                }

            }
            if(new_x + VAC_WIDTH == (screen_w))
            {

                if(vac_moving)
                {
                    new_head = target_object(screen_h,screen_w,vx,vy,rand()%screen_w,rand()%screen_h);
                }
            }

            if(vac_moving)
            {
                timer_pause( 500 );
                set_heading(new_head);
            }

        }
        //Check that next y position is within screen bounds
        //If not stop vacuum and change heading
        if(new_y == 0||new_y + VAC_HEIGHT == (screen_h-2)-VAC_HEIGHT/2)
        {
            vdx=vdy =0;

            if(new_y==0)
            {
                if(vac_moving)
                {
                    new_head = target_object(screen_h,screen_w,vx,vy,rand()%screen_w,rand()% (screen_h - 20 + 1) + 20);
                }
            }
            if(new_y + VAC_HEIGHT == (screen_h-2)-VAC_HEIGHT/2)
            {

                if(vac_moving)
                {
                    new_head = target_object(screen_h,screen_w,vx,vy,rand()%screen_w,5);
                }
            }
            if(vac_moving)
            {
                timer_pause( 500 );
                set_heading(new_head);
            }
        }
        drain_battery();
        go_to();
    }
}

/**
*   Function to place the vacuum in a new position
*/
void manual_reposition_vacuum (int x,int y,int direction)
{
    if(x > 1 && x < screen_w -(VAC_WIDTH+1))
    {
        vx=x;
    }
    else
    {
        if(x < 1)
        {
            vx=1;
        }
        if(x>screen_w -(VAC_WIDTH+1))
        {
            vx=screen_w -(VAC_WIDTH+1);
        }
    }
    if(y > 5 && y < screen_h -(VAC_HEIGHT+3))
    {
        vy=y;
    }
    else
    {
        if(y < 5)
        {
            vy=5;
        }
        if(y>screen_h -(VAC_HEIGHT+3))
        {
            vy=screen_h -(VAC_HEIGHT+3);
        }
    }
    set_heading(direction);
}

/** Updates width and height variables in vacuum.c from main program */
void update_screen_size(int width, int height)
{
    screen_w=width;
    screen_h=height;
}
