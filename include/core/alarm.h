#ifndef __ALARM__
#define __ALARM__

#include <common.h>

typedef void (*alarm_cb_t)(void);
void alarmsetup(uint16_t time_ms, alarm_cb_t alarm_cb);
void registeralarmcb(alarm_cb_t alarm_cb);
void timersetup(uint16_t time_ms);
void alarmshutdown(void);

#endif