/*
 * spi_xpt2046.c
 *
 * Created: 1/16/2018 7:31:26 AM
 *  Author: pvallone
 */ 
#include "spi_xpt2046.h"

void send_spi(uint8_t data){
	
	spi_select_device(MYSPI, &device);
	status_code_t status = 0;
	data_buffer[0] = data;
	
	status = spi_transceive_packet(MYSPI, data_buffer, read_buffer, sizeof(read_buffer));
	
	spi_deselect_device(MYSPI, &device);
	
	if(status == STATUS_ERR_TIMEOUT || status == ERR_TIMEOUT){
		printf("Time Error\n\r");
	}

}

uint8_t test_readxy(){
	
	int tmpH, tmpL,z1Range,z2Range;
	
	send_spi(0xD0);
	tmpH = (read_buffer[1] << 5);
	tmpL = (read_buffer[2] >> 3);
	tx =  tmpH | tmpL;
	
	send_spi(0x90);
	tmpH = (read_buffer[1] << 5);
	tmpL = (read_buffer[2] >> 3);
	ty =  tmpH | tmpL;

	if(tx > 0 && ty < z_threshold){
		printf("tx: %d ty: %d\n",tx,ty);
		//test our calculations
		z1Range = z1_high - z1_low;
		z2Range = z2_high - z2_low;

		float tempX1 = 0;
		float tempY1 = 0;
		
		if(tft_conf.tft_model == SSD1289 || tft_conf.tft_model == SSD1963_800 ){
			tempX1 = (((float)tft_width + 1) / z1Range);
			tempY1 = (((float)tft_height + 1) / z2Range);
		}else if(tft_conf.tft_model == SSD1963_800_5IN){
			tempX1 = (((float)tft_height + 1) / z1Range);
			tempY1 = (((float)tft_width + 1) / z2Range);
		}else if(tft_conf.tft_model == SSD1963_480){
			tempX1 = (((float)tft_height) / z1Range);
			tempY1 = (((float)tft_width) / z2Range);
		} else {
			tempX1 = (((float)tft_width + 1) / z1Range);
			tempY1 = (((float)tft_height + 1) / z2Range);
		}

		formatFloat(tempX1, calx);
		formatFloat(tempY1, caly);
		
		printf("Calculated Calibration Variables:\n");
		printf("tempX1 = %s \n",calx);
		printf("tempY1 = %s \n",caly);
		printf("z1Range = %d\n",z1Range);
		printf("z2Range = %d\n",z2Range);
		printf("z1_low = %d\n",z1_low);
		printf("z2_low = %d\n",z2_low);
		
		touch_x = abs(tempX1 * (float)(tx - z1_low));
		touch_y = abs(tempY1 * (float)(ty - z2_low));
		
		return 1;
		
	}

	return 0;
	
}

// use this just to grab the ranges (raw values)
uint8_t read_xy(){

	int tmpH, tmpL;

	send_spi(0xD0);
	tmpH = (read_buffer[1] << 5);
	tmpL = (read_buffer[2] >> 3);
	tx =  tmpH | tmpL;
	
	send_spi(0x90);
	tmpH = (read_buffer[1] << 5);
	tmpL = (read_buffer[2] >> 3);
	ty =  tmpH | tmpL;	

	if(tx > 0 && ty < z_threshold){
		
		//while(1){};
		if(step == 1){
			z1_low = tx;
		} else if (step == 2){
			z1_high = tx;
		} else if (step == 3){
			z2_low = ty;
		} else if (step == 4){
			z2_high = ty;
		}else {
			return 0;
		}
		delay_ms(200);
		return 1;
	}
	
	return 0;
	
}
