#include "hal.h"
#include "accel.h"

int main()
{
	int16_t result;
	uint8_t display;

	init();
	start_sampling();

	for(;;)
	{
		result=get_centrifugal_acceleration();

		if(result<0) // remove bogus negative values
		{
			result=0;
		}

		display = result >> 8; // heavily divide result (to get in range 0-7)

		// Make sure value is less than 8
		if(display>7)
		{
			display=7;
		}

		// Turn on corresponding LED
		display = 1 << display;
		print((uint8_t)display);
	}
	return 0;
}
