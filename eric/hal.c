#include "hal.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void error(uint8_t code);

uint8_t black[4]={0,0,0,0};

// Configure chip
void init()
{
	DDRB |= (1<<DDB3) | (1<<DDB5) | (1<<DDB2); // MOSI, SCK, and 3V_EN
	DDRD |= (1<<DDD6) | (1<<DDD7); // LED Enable and Latch
	DDRC |= (1<<DDC0); // Flash CS

	PORTC |= (1<<PC0); // Deselect flash

	SPCR |= (1<<SPE) | (1<<MSTR); // SPI Configuration
	SPSR |= (1<<SPI2X);

	PORTD |= (1<<PORTD7); // Latch

	PORTB &= ~(1<<PB2); // Turn power on to everything

	write_leds(black);
}

// Turn on the LEDs according to a 4-byte frame
void write_leds(uint8_t* data)
{
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

void i2c_start()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send I2C start condition
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which says start cond. has been transmitted
	if ((TWSR & 0xF8) != 0x08) { // 0x08 = start
		error(TWSR); // return status as error
	}
}

void i2c_repeated_start()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send I2C start condition
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which says start cond. has been transmitted
	if ((TWSR & 0xF8) != 0x10) { // 0x10 = repeated start sent
		error(TWSR); // return status as error
	}
}

void i2c_begin_write(uint8_t addr)
{
	TWDR = addr; // Write to addr
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates ACK received
	if ((TWSR & 0xF8) != 0x18) { // 0x18 = SLA+W sent, ACK received
		error(TWSR); // return status as error
	}
}

void i2c_begin_read(uint8_t addr)
{
	TWDR = addr | 1; // Read from addr
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates ACK received
	if ((TWSR & 0xF8) != 0x40) { // 0x40 = SLA+R sent, ACK received
		error(TWSR); // return status as error
	}
}

void i2c_write(uint8_t data)
{
	TWDR = data; // Output data
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates ACK received
	if ((TWSR & 0xF8) != 0x28) { // 0x28 = Data byte sent, ACK received
		error(TWSR); // return status as error
	}
}

void i2c_read_ack(uint8_t* data)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates byte received
	if ((TWSR & 0xF8) != 0x50) { // 0x50 = Data byte sent, ACK received
		error(TWSR); // return status as error
	}
	*data=TWDR;
}

void i2c_read_nack(uint8_t* data)
{
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates byte received
	if ((TWSR & 0xF8) != 0x58) { // 0x58 = Data byte sent, NACK received
		error(TWSR); // return status as error
	}
	*data=TWDR;
}

void i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send I2C stop condition
}

// Read a single byte from a register in the accelerometer
void accel_read_single(uint8_t addr,uint8_t reg,uint8_t* datum)
{
	i2c_start();
	i2c_begin_write(addr);
	i2c_write(reg);
	i2c_repeated_start();
	i2c_begin_read(addr);
	i2c_read_nack(datum);
	i2c_stop();
}

// Read two bytes from a register pair in the accelerometer
void accel_read_double(uint8_t addr,uint8_t reg,uint8_t* datum)
{
	i2c_start();
	i2c_begin_write(addr);
	i2c_write(reg);
	i2c_repeated_start();
	i2c_begin_read(addr);
	i2c_read_ack(&datum[1]);
	i2c_read_nack(&datum[0]);
	i2c_stop();
}

// Write a single byte to a register in the accelerometer
void accel_write_single(uint8_t addr,uint8_t reg,uint8_t datum)
{
	i2c_start();
	i2c_begin_write(addr);
	i2c_write(reg);
	i2c_write(datum);
	i2c_stop();
}

// Turn on red LEDs and wait forever
void error(uint8_t code)
{
	uint8_t frame[4];
	frame[0]=0;
	frame[1]=code;
	frame[2]=0;
	frame[3]=code;
	write_leds(frame);
	for(;;);
}

// Turn on green LEDs
void print(uint8_t code)
{
	uint8_t frame[4];
	frame[0]=code;
	frame[1]=0;
	frame[2]=code;
	frame[3]=0;
	write_leds(frame);
}

void power_on_periph()
{
	PORTB &= ~(1<<PB2); // Turn power on to everything
}

void power_off_periph()
{
	PORTB |= (1<<PB2); // Turn power off to everything
}

void uc_sleep()
{
	SMCR |= (1<<SM1) | (1<<SE); // Sleep mode is power-down, sleep enable
	sleep_cpu();
	while(PIND & (1<<PIND2)); // TODO: replace busy-wait with interrupt enable + WFI sleep

	/*
	EIMSK |= (1<<INT0); // Set INT0 external interrupt enable
	SMCR &= (1<<SM1) | (1<<SE); // Sleep mode is power-down, sleep enable
	sei();
	sleep_cpu();
	cli();
	SMCR &= ~(1<<SE);
	*/
}
