#include "accel.h"
#include "hal.h"

#define ACCEL_1		0x3A
#define ACCEL_2		0x38
#define OUT_Y		0x03
#define XYZ_DATA_CFG	0x0E

#define CTRL_REG1	0x2A
#define CTRL_REG2	0x2B
#define CTRL_REG3	0x2C
#define CTRL_REG4	0x2D
#define CTRL_REG5	0x2E
#define FF_MT_CFG	0x15
#define FF_MT_SRC	0x16
#define FF_MT_THS	0x17
#define FF_MT_COUNT	0x18

void accel_init()
{
	uint8_t result;
	accel_write_single(ACCEL_1,CTRL_REG2,0x40); // RESET
	accel_write_single(ACCEL_2,CTRL_REG2,0x40); // RESET
	for(;;)
	{
		accel_read_single(ACCEL_1,CTRL_REG2,&result);
		if(!(result & 0x40))
		{
			break;
		}
	}
	for(;;)
	{
		accel_read_single(ACCEL_2,CTRL_REG2,&result);
		if(!(result & 0x40))
		{
			break;
		}
	}

	accel_write_single(ACCEL_1,FF_MT_CFG,0xF8); // Motion detect
	accel_write_single(ACCEL_1,FF_MT_THS,0xC0); // Motion threshold at 2g
	accel_write_single(ACCEL_1,FF_MT_COUNT,0x01); // Debounce time at 80ms

	accel_write_single(ACCEL_1,CTRL_REG2,0x18); // Low power sleep mode
	accel_write_single(ACCEL_1,CTRL_REG3,0x08); // Wake up interrupt on motion
	accel_write_single(ACCEL_1,CTRL_REG4,0x04); // Motion interrupt enable
	accel_write_single(ACCEL_1,CTRL_REG5,0x04); // Motion interrupt to INT1	

	accel_write_single(ACCEL_1,XYZ_DATA_CFG,0x02); // 8g
}

// Turns on accelerometers
void accel_run()
{
	accel_write_single(ACCEL_1,CTRL_REG1,0x00); // Enter standby
	accel_write_single(ACCEL_2,CTRL_REG1,0x00);

	accel_write_single(ACCEL_2,XYZ_DATA_CFG,0x02); // 8g

	accel_write_single(ACCEL_1,CTRL_REG1,0x01); // Enter active, data rate = 800 Hz
	accel_write_single(ACCEL_2,CTRL_REG1,0x01);
}

void accel_sleep()
{
	uint8_t result;
	accel_write_single(ACCEL_1,CTRL_REG1,0x00); // Enter standby
//	accel_write_single(ACCEL_2,CTRL_REG1,0x00); // Enter standby
	accel_read_single(ACCEL_1,FF_MT_SRC,&result); // Clear pending interrupt
	accel_write_single(ACCEL_1,CTRL_REG1,0x29); // Enter active, data rate = 12.5 Hz
}

// Returns a 13-bit signed integer representing the centrifugal acceleration
int16_t get_centrifugal_acceleration()
{
	int16_t a,b;
	accel_read_double(ACCEL_1,0x03,(uint8_t*)(&a));
	accel_read_double(ACCEL_2,0x03,(uint8_t*)(&b));
	return (a>>5)+(b>>5);
}

