#include <stdint.h>


void init();
void write_leds(uint8_t* data);
void test();
void error(uint8_t code);
void print(uint8_t code);
void accel_read_single(uint8_t addr,uint8_t reg,uint8_t* datum);
void accel_write_single(uint8_t addr,uint8_t reg,uint8_t datum);
