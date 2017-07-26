/*
 * sam4s_i2c.c
 *
 * Created: 7/25/2017 8:09:28 AM
 *  Author: pvallone
 */ 
 #include <asf.h>
 #include "sam4s_i2c.h"


 void sam4s_configure_i2c(void){

	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	uint32_t status;
	pmc_enable_periph_clk(ID_TWI0);
	//set options
	twi_master_options_t opt;

	opt.speed = TWI_SPEED;
	opt.chip = (0x68 >> 1);
	opt.master_clk = sysclk_get_cpu_hz();
	// Init TWI Master
	status = twi_master_setup(TWI_CHANNEL, &opt);
	if (status != TWI_SUCCESS)
	{
		while(1); // wait to finish
	}
	
	twi_master_enable(TWI_CHANNEL);
 }

void sam4s_i2c_scan(void){

     printf("Scan started....\n");
	 read_buffer[0] = 0x00;
	 for(uint8_t address = 1; address < 127; address++ ) {	 
		 
		 scan_packet.chip  = address;
		 scan_packet.length = 1;
		 scan_packet.buffer = &read_buffer;

		 uint32_t result = twi_master_write(TWI_CHANNEL, &scan_packet);
		 
		 if (result == TWI_SUCCESS)
		 {
			 printf("I2C device found at decimal address Hex address is 0x%x\n", address);
			 
		 }

	 }
	  printf("Scan completed....\n");
}

