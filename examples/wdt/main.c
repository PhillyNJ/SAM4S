#include <asf.h>
#include "conf_usart.h"
#include "conf_wdt.h"

// interrupt
void configure_interupts(void);
void pin_edge_handler(const uint32_t id, const uint32_t index);

volatile bool g_b_systick_event = false;
volatile uint32_t g_ul_ms_ticks = 0;
volatile uint8_t g_b_button_event = false;
#define SW1 PIO_PA14
#define BLINK_PERIOD 500

int main (void)
{
	board_init(); // delete the option to disable the WDT here or WDT will not work
	sysclk_init();	// don't for get this!
	configure_uart(115200);
	configure_wdt();
	configure_interupts();
	SysTick_Config(sysclk_get_cpu_hz() / 1000); // for system tick
	delay_init();	
	
	while (1) {
		
		if (g_b_systick_event == true) {

			g_b_systick_event = false;
			if ((g_ul_ms_ticks % WDT_RESTART_PERIOD) == 0) { /* Restart watchdog timer after 9000 (9 seconds)ticks. */
				wdt_restart(WDT);
			}
			/* put all your code here */		
			if ((g_ul_ms_ticks % BLINK_PERIOD) == 0) {
				LED_Toggle(LED0_GPIO);
			}			
		}

		if(g_b_button_event==true){
			printf("Simulate deadlock\n");
			while (1) {
			}
		}
		//if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			///* Yes, so turn LED on. */
			//ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		//} else {
			///* No, so turn LED off. */
			//ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		//}
	}
}

void configure_interupts(void){

	pmc_enable_periph_clk(ID_PIOA);
	pio_set_input(PIOA, SW1, PIO_PULLUP);
	pio_set_debounce_filter(PIOA,  SW1, 20);
	pio_handler_set(PIOA, ID_PIOA, SW1, PIO_IT_EDGE, pin_edge_handler);
	pio_enable_interrupt(PIOA, SW1);
	NVIC_EnableIRQ(PIOA_IRQn);
}

void pin_edge_handler(const uint32_t id, const uint32_t index)
{
	if ((id == ID_PIOA) && (index == SW1)){
		if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, SW1) ){
			printf("Button PA14 Pressed to trigger deadlock\n\r");
			g_b_button_event = true;
		}
	}
}
