#include "accel.h"
#include "hal.h"


void sleep()
{
	accel_sleep(); // Doing this before power_off is oddly important
	power_off_periph();
	uc_sleep();
	power_on_periph();
	accel_run();
}
