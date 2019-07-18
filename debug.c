#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


static void timestamp(FILE *f);

// https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
static void timestamp(FILE *f)
{
    char buffer[26];
    int millisec;
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000)   // Allow for rounding up to nearest second
    {
        millisec -=1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
    fprintf(f,"%s.%03d\n", buffer, millisec);
}

void dtf_int(char * string, int value)
{

    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,value);
    fclose(f);
}

void dtf_int2(char * string, int value1,int value2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,value1,value2);
    fclose(f);
}

void dtf_double(char * string, double value)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,value);
    fclose(f);
}

void dtf_double2(char * string, double value1,double value2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,value1,value2);
    fclose(f);
}

void dtf_mixed_int_double(char * string, int value1,double value2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,value1,value2);
    fclose(f);
}

void dtf_mixed_int_double2(char * string, int int1,double double1,double double2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,string,int1,double1,double2);
    fclose(f);
}
void dtf_mixed_str_int_double2(char * params, char* string, int int1,double double1,double double2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,params,string,int1,double1,double2);
    fclose(f);
}
void dtf_mixed_int2_double2(char * params, int int1, int int2,double double1,double double2)
{
    FILE *f = fopen("debug.txt","a");
    fprintf(f,params,int1,int2,double1,double2);
    fclose(f);
}

void dtf_string(char * string,bool print_timestamp)
{
    FILE *f = fopen("debug.txt","a");
    if(print_timestamp)
    {
        timestamp(f);
    }
    fprintf(f,string);
    fclose(f);
}

void debug_double_array2(double array1[],double array2[],int count)
{
    FILE *f = fopen("debug.txt","a");
    for(int i=0; i<=count; i++)
    {
        fprintf(f,"ArrayPos %d = (%f,%f)\n",i,array1[i],array2[i]);
    }
    fclose(f);
}

void clear_debug_file(void)
{
    FILE *f = fopen("debug.txt","w");
    fprintf(f," ");
    fclose(f);
}
