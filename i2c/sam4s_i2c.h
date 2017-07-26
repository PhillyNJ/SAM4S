/*
 * sam4s_i2c.h
 *
 * Created: 7/25/2017 8:09:18 AM
 *  Author: pvallone
 */ 


#ifndef SAM4S_I2C_H_
#define SAM4S_I2C_H_


#define TWI_CHANNEL				TWI0
#define TWI_SPEED				400000 
#define I2C_TIMEOUT				1000

twi_packet_t write_packet;
twi_packet_t read_packet;
twi_packet_t scan_packet;

uint8_t read_buffer[10];
uint8_t write_buffer[10];
void sam4s_configure_i2c(void);
void sam4s_i2c_scan(void);


#endif /* SAM4S_I2C_H_ */