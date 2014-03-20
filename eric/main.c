#include "hal.h"

int main()
{
	int16_t result;
	uint8_t display;

	init();
	start_sampling();

	for(;;)
	{
		result=get_centrifugal_acceleration();
		/*
		if(result<0)
		{
			result=-result;
		}
		*/
		display = result >> 7; // heavily divide result
		if(display>7)
		{
			display=7;
		}
		display = 1 << display;
		print((uint8_t)display);
	}
	return 0;
}
