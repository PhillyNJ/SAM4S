/* DS3231 RTC with SAM4S1  */

/* Notes: 
 * SDA -> PA03
 * SCL -> PA04
 * SQW -> PA14 (Interrupt)
 * Requires the following additional modules
 *	- TWI - Two Wire interface - SAM implementation
 *	- USART - Serial interface - SAM implementation for both UART and USART
 *	- Standard I/O (stdio) 
 *	- Delay routines
*/

#include <asf.h>
#include "conf_usart.h"
#include "sam4s_i2c.h"
#include "ds3231.h"

#define SQW PIO_PA14

void formatFloat(float f, char * buffer);
void checkAlarms(void);
void test_polling_method_alarm1(void);
void test_polling_method_alarm2(void);
void configure_alarm_interrupt(uint8_t alarm);
void pin_edge_handler(const uint32_t id, const uint32_t index);
RTCDateTime dt;
char temp_buffer[20];
uint8_t alarm_to_trigger;
bool alarm_state;


int main (void)
{
	sysclk_init();
	board_init();
	configure_uart(115200);
	ds3231_init();
	delay_init();
	printf("SAM4S DS3231 Example\n");
	printf(("Compile time %s \n"), __TIME__);
	printf(("Compile Date %s \n"), __DATE__);
	// Set the time when compiling
	//ds3231_set_date_time_from_compile_time(__DATE__, __DATE__); 

	// disarm both alarms
	ds3231_arm_alarm_1(false);
	ds3231_arm_alarm_2(false);

	configure_alarm_interrupt(2);

	/* polling alarm 2*/
	//ds3231_clear_alarm_2();
	//ds3231_set_alarm_2(0, 0, 23, DS3231_MATCH_M, true);
	
	/* polling alarm 1*/
	//ds3231_clear_alarm_1();
	//ds3231_set_alarm_1(0, 0, 0, 37, DS3231_MATCH_S, true);

	char month[15];
	char weekday[15];

	while (1) {
		
		ds3231_getDateTime(&dt);
				
		printf("Time:\t%02d:%02d:%02d\n", dt.hour, dt.minute,dt.second);
		printf("Date:\t%d.%d.%d\n", dt.month, dt.day,dt.year);
		ds3231_get_month_str(dt.month, month);
		ds3231_get_dow_str(dt.dayOfWeek, weekday);
		printf("Month\t%s\n", month);
		printf("Day:\t%s\n", weekday);
		
		float temp = ds3231_read_temperature();
		formatFloat(temp, temp_buffer);
		printf("Temp:\t%s c\n", temp_buffer);
		float ftemp = temp * 1.8 + 32;
		formatFloat(ftemp, temp_buffer);
		printf("Temp:\t%s F\n", temp_buffer);
		//test_polling_method_alarm2();

		delay_s(1);
		checkAlarms();
		/* Is button pressed? */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Yes, so turn LED on. */
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			// alarm polling method reset
			//ds3231_clear_alarm_1();
			ds3231_clear_alarm_2();
			//checkAlarms(); // or check the status of the alarms
		} else {
			/* No, so turn LED off. */
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}

		/* interrupt*/
		if(alarm_state){
			alarm_to_trigger == 1 ? printf("Alarm 1 Fired \n\r") : printf("Alarm 2 Fired \n\r");
			alarm_state = false;
		}


	}
}

void test_polling_method_alarm1(void){

	checkAlarms();	//
	// polling method
	if(ds3231_check_alarm_1_state(false)){
		printf("***** Alarm 1 went off!!\n");
	}

}

void test_polling_method_alarm2(void){

	checkAlarms();
	// polling method
	if(ds3231_check_alarm_2_state(false)){
		printf("***** Alarm 2 went off!!\n");
	}

}
void checkAlarms()
{
	RTCAlarmTime a1;
	RTCAlarmTime a2;

	if (ds3231_is_armed_1())
	{
		ds3231_get_alarm_1(&a1);

		printf("Alarm1 is triggered ");

		DS3231_alarm1_t mode = ds3231_get_alarm_type_1();
		switch (mode)
		{
			case DS3231_EVERY_SECOND:
			printf("every second");
			break;
			case DS3231_MATCH_S:
			printf("when seconds match: ");
			printf("%02d seconds\n", a1.second);
			break;
			case DS3231_MATCH_M_S:
			printf("when minutes and seconds match: ");
			printf("%02d:%02d\n", a1.minute, a1.second);
			break;
			case DS3231_MATCH_H_M_S:
			printf("when hours, minutes and seconds match: ");
			printf("%02d:%02d:%02d\n", a1.hour, a1.minute, a1.second);
			break;
			case DS3231_MATCH_DT_H_M_S:
			printf("when date, hours, minutes and seconds match: ");
			printf("%d  %02d:%02d:%02d\n", a1.day, a1.hour, a1.minute, a1.second);
			break;
			case DS3231_MATCH_DY_H_M_S:
			printf("when day of week, hours, minutes and seconds match: ");
			printf("%d  %02d:%02d:%02d\n", a1.day, a1.hour, a1.minute, a1.second);
			break;
			default:
			printf("UNKNOWN RULE");
			break;
		}

		printf("\n");
	} else
	{
		printf("Alarm1 is disarmed.\n");
	}

	if (ds3231_is_armed_2())
	{
		ds3231_get_alarm_2(&a2);

		printf("Alarm2 is triggered ");
		switch (ds3231_get_alarm_type_2())
		{
			case DS3231_EVERY_MINUTE:
			printf("every minute");
			break;
			case DS3231_MATCH_M:
			printf("when minutes match: ");
			printf("%02d\n", a2.minute);
			break;
			case DS3231_MATCH_H_M:
			printf("when hours and minutes match:");
			printf("%02d:%02d\n", a2.hour, a2.minute);
			break;
			case DS3231_MATCH_DT_H_M:
			printf("when date, hours and minutes match: ");
			printf("%d %02d:%02d\n", a2.day, a2.hour, a2.minute);
			break;
			case DS3231_MATCH_DY_H_M:
			printf("when day of week, hours and minutes match: ");
			printf("%d %02d:%02d\n", a2.day, a2.hour, a2.minute);
			break;
			default:
			printf("UNKNOWN RULE");
			break;
		}
		printf("\n");
	} else
	{
		printf("Alarm2 is disarmed.\n");
	}
}


void configure_alarm_interrupt(uint8_t alarm){
	pmc_enable_periph_clk(ID_PIOA);

	pio_set_input(PIOA, SQW, PIO_PULLUP);
	pio_set_debounce_filter(PIOA,  SQW, 20);
	pio_handler_set(PIOA, ID_PIOA, SQW, PIO_IT_LOW_LEVEL, pin_edge_handler);
	pio_enable_interrupt(PIOA, SQW);
	NVIC_EnableIRQ(PIOA_IRQn);

	alarm_to_trigger = alarm;
	if(alarm == 1){

		/* alarm 1 - will trigger the interrupt when second reaches 1, then shut off, then repeat when seconds reaches 1.	*/
		ds3231_arm_alarm_1(false);
		ds3231_clear_alarm_1();
		ds3231_enable_output(true); // set he square wave output
		ds3231_set_alarm_1(0, 0, 0, 01, DS3231_MATCH_S, true);
	} else if(alarm == 2){

		/* alarm 2 - will trigger the interrupt when minutes is 31 then shut off, then repeat*/
		ds3231_arm_alarm_2(false);
		ds3231_clear_alarm_2();
		ds3231_enable_output(true); // set he square wave output
		ds3231_set_alarm_2(0, 0, 31, DS3231_MATCH_M, true);
	}


}

void pin_edge_handler(const uint32_t id, const uint32_t index)
{
	 if ((id == ID_PIOA) && (index == SQW)){
		 alarm_to_trigger == 1 ? ds3231_clear_alarm_1() : ds3231_clear_alarm_2();	
		 alarm_state = true;	
		
	 }
}


void formatFloat(float f, char * buffer){
	
	int d1 = f;  // Get the integer part.
	float f2 = f - d1;     // Get fractional part
	int d2 = f2 * 10000;   // Turn into integer
	sprintf (buffer, "%d.%04d", d1, d2);
	
}