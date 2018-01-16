/*
 * spi_xpt2046.h
 *
 * Created: 1/16/2018 7:31:13 AM
 *  Author: pvallone
 */ 


#ifndef SPI_XPT2046_H_
#define SPI_XPT2046_H_
#include <asf.h>
#include "spi_master.h"
#include "sam_tft.h"
#include "utils.h"
// spi
#define SPI_CLOCK_SPEED		4000000UL
#define MYSPI				SPI
#define BUFFERSIZE			3
#define READBUFFERSIZE		3
// temp vars (dummy values)
#define CALX				0.0675f
#define CALY				0.0822622107969152f + 0.090625f
#define XRANGE				3779
#define YRANGE				3800

int z_threshold, oldx, oldy,step;
int tx,ty,z1_low,z1_high,z2_low,z2_high;

char calx[20];
char caly[20];

float touch_x,touch_y;

uint8_t data_buffer[BUFFERSIZE];
uint8_t read_buffer[READBUFFERSIZE];
struct spi_device device;
// how to calculate x,y -> https://www.youtube.com/watch?v=4mR_h87MlhQ
uint8_t read_xy(void);
uint8_t test_readxy(void);
void send_spi(uint8_t data);
#endif /* SPI_XPT2046_H_ */