
/**
 * Example DS1302 with SAM4S xplained pro

 *	Pin Out - To change pinout to different pins, see ds1302.h
 *	CE ->	PIO_PA23
 *  DATA ->	PIO_PA24
 *  SCLK->	PIO_PA25
 *  If you change the pins to different port/group, you need to update any register calls 
 *  
 */


#include <asf.h>
#include "conf_usart.h"
#include "ds1302.h"

int main (void)
{
	sysclk_init();
	board_init();
	configure_uart(115200);
	printf("SAM4S DS1302 Example\n");
	printf(("Compile time %s \n"), __TIME__);
	printf(("Compile Date %s \n"), __DATE__);
	ds1302_init(); 
	ds1302_halt(false);	

	//ds1302_write_protect(false);	
	//ds1302_set_compile_time(SATURDAY); // always set to compile time :)

	ds_time_t myTime;

	while (1) {
		ds1302_get_complete_time(&myTime);
		printf("It's %s!\n", myTime.monl);
		printf("Today is %s %d.%d.%d\n", myTime.dowl, myTime.mon, myTime.day, myTime.year);
		printf("Time is %02d:%02d:%02d\n", myTime.hour, myTime.min, myTime.sec);
		delay_s(1);

	}
}
