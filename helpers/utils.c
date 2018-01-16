/*
 * utils.c
 *
 * Created: 1/16/2018 7:28:04 AM
 *  Author: pvallone
 */ 
 #include "utils.h"

 void formatFloat(float f, char * buffer){
	 
	 int d1 = f;  // Get the integer part.
	 float f2 = f - d1;     // Get fractional part
	 int d2 = f2 * 1000000000;   // Turn into integer
	 sprintf (buffer, "%d.%06d", d1, d2);
	 
 }