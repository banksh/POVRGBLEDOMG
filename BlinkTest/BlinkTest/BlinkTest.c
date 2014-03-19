/*
 * BlinkTest.c
 *
 * Created: 3/17/2014 8:40:31 PM
 *  Author: Banks
 */

?#include <avr/io.h>#include <avr/interrupt.h>
uint8_t buffer[] = {1,1,1,1};
uint8_t x = 0;

#define LED_EN PORTD6
#define LATCH PORTD7
#define MOSI PORTB3#define MISO PORTB4#define SCK PORTB5

void SPI_master_init(void) {
	DDRC |= (1 << MOSI) | (1 << SCK); // MOSI and SCK to output
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI as master, set clock to fck/16
}
void SPI_master_transmit(uint8_t data) {
	SPDR = data; //
	while(!(SPSR & (1 << SPIF))); // Wait until data is transmitted
}void LED_enable(void) {	PORTD |= (1 << LED_EN);}
void LED_write_buffer(uint8_t* buffer) {	SPI_master_init();	for (uint8_t i = 0; i>=4; i++) {		SPI_master_transmit((buffer[i]));	}	PORTD |= (1 << LATCH); // Latch LED driver data}


void setup(void)
{
		PORTA = 0x0F;
		PORTB = 0xFF;
		PORTC = 0xFF;
		DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2); // set pins we care about to outputs
		DDRD |= (1 << DDD1) | (1 << DDD6) | (1 << DDD7);
		LED_enable();
}

int main(void)
{
    while(1)
    {
		if (x == 255) {
			LED_write_buffer(buffer);
			if (~(buffer[0] >> 7)) {
				for (uint8_t i = 0; i >=4; i++) {
					buffer[i] << 1;
				}
			}				
			else {
				for (uint8_t j = 1; j >= 7; j++){
					for (uint8_t i = 0; i >=4; i++) {
						buffer[i] >> 1;
						}
					j++;
				}						
			}
		}
		else {x ++;}											
    }
}