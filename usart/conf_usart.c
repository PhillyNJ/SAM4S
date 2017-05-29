/*
 * conf_usart.c
 *
 * Created: 5/26/2017 7:16:41 AM
 *  Author: pvallone
 */ 
 #include "conf_usart.h"

 void configure_uart(uint32_t baud){

	 pio_configure(PINS_UART1_PIO, PINS_UART1_TYPE, PINS_UART1_MASK, PINS_UART1_ATTR);
	 pmc_enable_periph_clk(ID_UART1);
	 
	 const sam_uart_opt_t uart1_settings =
	 { sysclk_get_cpu_hz(), baud, UART_SERIAL_MODE };

	 uart_init(UART1,&uart1_settings);      //Init UART1 and enable Rx and Tx

	 uart_enable_interrupt(UART1,UART_IER_RXRDY);   //Interrupt reading ready
	 NVIC_EnableIRQ(UART1_IRQn);
	 
	 // hook it to stdio
	 const usart_serial_options_t uart_serial_options = {
		 .baudrate =  baud

	 };
	 
	 sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	 stdio_serial_init(CONSOLE_UART, &uart_serial_options);
 }

 void UART1_Handler(){
	 uint8_t rx_data;
	 uint32_t status = uart_get_status(UART1);
	 
	 if(status & UART_SR_RXRDY){
		 //read
		 uart_read(UART1, &rx_data);
		 
		 // handle any data
		 if(rx_data == 'L'){
			 ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			 delay_ms(300);
		 }
		 
		 //reply back the same
		 while (!(UART1->UART_SR & UART_SR_TXRDY));
		 
		 
		 uart_write(UART1, rx_data); //send data
	 }
 }
