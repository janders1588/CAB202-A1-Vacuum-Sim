#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

void dtf_int(char * string, int value);
void dtf_int2(char * string, int value1,int value2);
void dtf_double(char * string, double value);
void dtf_double2(char * string, double value1,double value2);
void dtf_string(char * string, bool print_timestamp);
void clear_debug_file(void);
void debug_double_array2(double array1[],double array2[],int count);
void dtf_mixed_int_double(char * string, int value1,double value2);
void dtf_mixed_int_double2(char * string, int int1,double double1,double double2);
void dtf_mixed_str_int_double2(char * params, char* string, int int1,double double1,double double2);
void dtf_mixed_int2_double2(char * params, int int1, int int2,double double1,double double2);
