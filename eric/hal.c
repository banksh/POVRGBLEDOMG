#include "hal.h"
#include <avr/io.h>

void init()
{
	DDRB |= (1<<DDB3) | (1<<DDB5) | (1<<DDB2); // MOSI, SCK, and SS
	DDRD |= (1<<DDD6) | (1<<DDD7); // LED Enable and Latch

	SPCR |= (1<<SPE) | (1<<MSTR); // SPI Configuration

	PORTD |= (1<<PORTD7); // Latch
}

void write_leds(uint8_t* data)
{
	uint8_t i;

	SPDR = data[0];
	while(!(SPSR & (1<<SPIF)));
	SPDR = data[1];
	while(!(SPSR & (1<<SPIF)));
	SPDR = data[2];
	while(!(SPSR & (1<<SPIF)));
	SPDR = data[3];
	while(!(SPSR & (1<<SPIF)));

	PORTD |= (1<<PORTD7);
	PORTD &= ~(1<<PORTD7);
}

void test()
{
	int16_t pause;
	uint8_t frame[4];
	int8_t i;

	for(;;)
	for(i=0;i<8;i++)
	{
		frame[0]=1<<i;
		frame[1]=1<<(7-i);
		frame[2]=frame[0];
		frame[3]=frame[1];
		write_leds(frame);
		for(pause=0;pause<10000;pause++);
	}

}

