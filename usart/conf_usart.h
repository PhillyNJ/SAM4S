/*
 * conf_usart.h
 *
 * Created: 5/26/2017 7:16:31 AM
 *  Author: pvallone
 */ 


#ifndef CONF_USART_H_
#define CONF_USART_H_

#include <asf.h>
#define UART_SERIAL_MODE         UART_MR_PAR_NO

#define PINS_UART1_PIO			PIOB		  //Rx and Tx pins are in Parallel IO B
#define PINS_UART1_ID			ID_UART1
#define PINS_UART1_TYPE			PIO_PERIPH_A  //UART1 is part of Peripheral A pg. 762 SAM4S Data sheet
#define PINS_UART1_ATTR			PIO_DEFAULT
#define PINS_UART1_MASK			(PIO_PB2A_URXD1| PIO_PB3A_UTXD1 )

void configure_uart(uint32_t baud);


#endif /* CONF_USART_H_ */