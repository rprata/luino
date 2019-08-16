#include <hserial.h>
#include <adc.h>
#include <io.h>
#include <alarm.h>
#include <pwm.h>

#define ALARM_TIMER_MS 50

volatile bool val = false;
volatile uint16_t analog = 0;
volatile state_t s = low;
volatile pwm_st * pwm;
float i = 0.0;

void read_async(state_t state) {
	iowriteoutput(PIN13, state);
}

void loop() {
	// val = ioreadinput(PIN12);
	// analog = adcread(A0);
	// if (val || analog > 512) {
	// 	iowriteoutput(PIN13, high);	
	// } else {
	// 	iowriteoutput(PIN13, low);
	// }
	pwmduty(pwm, i);
	i += 0.001;
	if (i >= 1.0)	i = 0.0;

}

int main(void) {
#ifdef ENABLE_LUA
	luino_init();
#endif
	iosetpinnout(PIN13, output);
	iosetpinnout(PIN9, input);
	ioreadasync(PIN9, read_async);
	// adcsetup(adc_resolution_10, adc_prescaler_32);
	// pwm = pwmsetup(PIN3, pwm_prescaler_none, 0.0);
	// alarmsetup(ALARM_TIMER_MS, loop);
	for (;;) {
		// loop();
		_delay_ms(ALARM_TIMER_MS);
	}

	return 0;
}