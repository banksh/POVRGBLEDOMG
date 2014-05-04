#include "accel.h"
#include "hal.h"

#define ACCEL_1		0x38
#define ACCEL_2		0x3A
#define CTRL_REG1	0x2A
#define OUT_Y		0x03
#define XYZ_DATA_CFG	0x0E

// Turns on accelerometers
void start_sampling()
{
	accel_write_single(ACCEL_1,CTRL_REG1,0x01);
	accel_write_single(ACCEL_2,CTRL_REG1,0x01);
	accel_write_single(ACCEL_1,XYZ_DATA_CFG,0x02);
	accel_write_single(ACCEL_2,XYZ_DATA_CFG,0x02);
}

// Returns a 13-bit signed integer representing the centrifugal acceleration
int16_t get_centrifugal_acceleration()
{
	int16_t a,b;
	accel_read_double(ACCEL_1,0x03,(uint8_t*)(&a));
	accel_read_double(ACCEL_2,0x03,(uint8_t*)(&b));
	return (a>>5)+(b>>5);
}
