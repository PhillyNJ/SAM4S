/*
 * conf_wdt.h
 *
 * Created: 5/29/2017 8:43:56 AM
 *  Author: pvallone
 */ 


#ifndef CONF_WDT_H_
#define CONF_WDT_H_

#include <asf.h>

/* WDT*/
extern volatile bool g_b_systick_event;
extern volatile uint32_t g_ul_ms_ticks;

#define WDT_PERIOD                        15000
#define WDT_RESTART_PERIOD                9000
void configure_wdt(void);


#endif /* CONF_WDT_H_ */