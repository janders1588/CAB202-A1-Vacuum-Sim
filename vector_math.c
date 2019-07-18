#include <math.h>
#include <cab202_graphics.h>

static double get_angle(double h, double w, double ax,double ay,double bx,double by);

double degree_to_radian(double degree)
{
    return degree*M_PI/180;
}

static double radian_to_degree(double cos)
{
    return cos*180/M_PI;
}

static double get_angle(double h, double w, double ax,double ay,double bx,double by)
{
    double x1 = ax-(h/2);
    double y1 = ay-(w/2);
    double x2 = bx-(h/2);
    double y2 = by-(w/2);
    double y = (y2-y1);
    double x = (x2-x1);
    double angle = atan2(y,x);
    return angle;
}

int target_object(double h, double w, double x1, double y1, double x2, double y2)
{
    double angle = get_angle(h,w,x1,y1,x2,y2);
    double heading  = radian_to_degree(angle);

    return round(heading);
}
