#include "hal.h"
#include "accel.h"
#include "power.h"

void pause();
void text_display();

int main()
{
	int16_t result;
	uint8_t display;

	uint8_t pattern[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x08,0x04,0x02};
	//uint8_t pattern[] = {0x42,0x86,0x8a,0x92,0x62,0x00,0x02,0x00,0x7c,0x82,0x82,0x82,0x7c,0x00,0x7c,0x82,0x82,0x82,0x7c,0x00,0x6c,0x92,0x92,0x92,0x6c,0x00,0x00,0x00,0x00};
	uint8_t pat_len = 14;
	//uint8_t pat_len = 30;
	uint8_t i=pat_len;
	uint8_t frame[4];

	uint16_t j;
	int16_t ca;
	
	int16_t x1;
	int16_t x2;
	int16_t y1;
	int16_t y2;

	init();
	accel_init();

	for(;;)
	{
		sleep();
		for(;;)
		{
			frame[0]=pattern[i];
			frame[2]=pattern[i];
			frame[1]=pattern[i];
			frame[3]=pattern[i];

			if(i==0)
			{
				x1 = get_accel_1_x();
				x2 = get_accel_2_x();
				y1 = get_accel_1_y();
				y2 = get_accel_2_y();
				ca = (x1>>5)+(x2>>5);
				//ca=get_centrifugal_acceleration();
				if ((x1>>5)>25 || (y1>>5)>25)
				{
				text_display();
				}
				if(ca<50)
				{
					break;
				}
				i=pat_len;
			}
			write_leds(frame);
			i--;
		}
	}

	return 0;
}

void pause()
{
	uint16_t i;
	for(i=0;i<1000;i++)
	{
	}
}

void text_display()
{
	frame[0]=0xFF;
	frame[1]=0xFF;
	frame[2]=0xFF;
	frame[3]=0xFF;
	write_leds(frame);
}
