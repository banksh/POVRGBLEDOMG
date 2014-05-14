#include "accel.h"
#include "hal.h"

void sleep()
{
	power_off_periph();
	accel_standby();
	uc_sleep();
	power_on_periph();
	accel_run();
}
