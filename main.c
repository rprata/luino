#include <hserial.h>
#include <adc.h>
#include <io.h>
#include <alarm.h>
#include <pwm.h>

#define SLEEP_TIME 2000

int main(void) {
	iosetpinnout(PIN13, output);
#ifdef ENABLE_LUA
	luino_init("print(\"Hello, world.\")");
#endif
	for (;;) {
		iowriteoutput(PIN13, low);	
		_delay_ms(SLEEP_TIME);
		iowriteoutput(PIN13, high);	
		_delay_ms(SLEEP_TIME);
	}

	return 0;
}