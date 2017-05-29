/*
 * conf_wdt.c
 *
 * Created: 5/29/2017 8:44:04 AM
 *  Author: pvallone
 */ 
 #include "conf_wdt.h"

 
 void SysTick_Handler(void)
 {
	 /* Set systick event flag (g_b_systick_event) and add 1 to systick. */
	 g_b_systick_event = true;
	 g_ul_ms_ticks++;

 }
 void configure_wdt(void){
	 
	 uint32_t wdt_mode, timeout_value;
	 /* Get timeout value. */
	 timeout_value = wdt_get_timeout_value(WDT_PERIOD * 1000, BOARD_FREQ_SLCK_XTAL);
	 if (timeout_value == WDT_INVALID_ARGUMENT) {
		 while (1) {
			 /* Invalid timeout value, error. */
			 printf("Invalid Timeout!!!%ld\n", timeout_value);
		 }

	 }
	 printf("Timeout:\t%ld\n", timeout_value);
	 /* Configure WDT to trigger an interrupt (or reset). */
	 wdt_mode = WDT_MR_WDFIEN |  /* Enable WDT fault interrupt. */
	 WDT_MR_WDRSTEN	 |  /* WDT restart enabled	*/
	 WDT_MR_WDRPROC   |  /* WDT fault resets processor only. */
	 WDT_MR_WDDBGHLT  |  /* WDT stops in debug state. */
	 WDT_MR_WDIDLEHLT;	/* WDT stops in idle state. */
	 
	 wdt_init(WDT, wdt_mode, timeout_value, timeout_value);

	 printf("WTD Reg: 0x%x\n", (unsigned int) WDT->WDT_MR);

	 printf("Enable watchdog with %d microseconds period\n\r",	(int)wdt_get_us_timeout_period(WDT, BOARD_FREQ_SLCK_XTAL));

	 /* Configure and enable WDT interrupt. */
	 NVIC_DisableIRQ(WDT_IRQn);
	 NVIC_ClearPendingIRQ(WDT_IRQn);
	 NVIC_SetPriority(WDT_IRQn, 0);
	 NVIC_EnableIRQ(WDT_IRQn);
	 
 }
 void WDT_Handler(void)
 {
	 printf("Enter watchdog interrupt.\r");
	 /* Clear status bit to acknowledge interrupt by dummy read. */
	 wdt_get_status(WDT);
	 /* Restart the WDT counter. */
	 wdt_restart(WDT);
	 printf("The watchdog timer was restarted.\r");
	 g_b_systick_event = true;
 }