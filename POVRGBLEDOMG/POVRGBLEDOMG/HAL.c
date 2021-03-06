﻿#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define MOSI PORTB3
#define MISO PORTB4
#define SCK PORTB5
#define CS PORTC0 // flash memory chip select (high off, low on)
#define HOLD PORTC1 // flash memory data hold (#don'tcare when low)
#define WP PORTC2 // flash memory write protect when low
#define SDA PORTC4
#define SCL PORTC5
#define IR PIND0 // IR transistor measure (input)
#define IR_DRIVE PORTD1 // IR transistor drive pin (high to drive)
#define LED_EN PORTD6 // LED driver enable (high off, low on)
#define LATCH PORTD7 // LED driver data latch (low to latch)
#define acc1_addr 0x38 // read address, add 1 for write
#define acc2_addr 0x3A

void setup(){
	sei(); // global interrupt enable
	
	MCUSR &= ~(1 << WDRF); // clear watchdog system reset flag in case WDT caused reset
	WDTCSR &= ~(1 << WDE); // clear watchdog system reset enable

	PRR |= (1 << PRTWI) | (1 << PRTIM0) | (1 << PRTIM1) | (1 << PRSPI) | (1 << PRADC); // shut down clocks to everything (TWI, TIM0, TIM1, SPI, ADC)
	ACSR |= (1 << ACD); // disable analog comparator
	
	EICRA = 0b00000101; // trigger interrupts INT0 and INT1 on any logical change
	
	// enable pull-ups on GPIO to reduce power consumption
	PORTA = 0x0F;
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = ~((1 << IR) | (1 << IR_DRIVE)); // default to having IR with no pull-up and IR_DRIVE low
	
	DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2); // set pins we care about to outputs
	DDRD |= (1 << DDD1) | (1 << DDD6) | (1 << DDD7);
}

void one_sec_sleep() {
	cli(); // disable interrupts

	//Enable watchdog timer interrupts,
	//configure watchdog timer in interrupt
	//mode, set watchdog clock prescale for
	//1 second time-out 
	WDTCSR |= (1 << WDCE) | (1 << WDE); // start timed watchdog sequence
	WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP1); // set prescale for 1 second time-out
	sei();
}

void sleep(){
	SMCR = (1 << SE); // sleep enable
	asm("nop"); // wait a tick before entering sleep
	SMCR |= (1 << SM1);
	SMCR &= ~(1 << SM0); // full power down
}

void idle(){
	SMCR = (1 << SE);
	asm("nop");
	SMCR &= ~((1 << SM1) | (1 << SM0)); // idle mode
}

uint8_t read_accel_data(uint8_t addr) {
	I2C_init();
	I2C_data_write(addr&1); // change addr to read address (LSB -> 1), write it
	I2C_data_read();
	
}

uint8_t I2C_init() {
	PRR &= ~(1 << PRTWI); // enable TWI (I2C) clock
	TWCR = (1 << TWINT) | (1 << TWSTA) | ( << TWEN); // send I2C start condition
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which says start cond. has been transmitted
	if ((TWSR & 0x08) != 0x08) { // if the status register isn't 0x08 (START condition transmitted)
		error(TWSR); // return status as error
	}
}

uint8_t I2C_data_write(uint8_t data) {
	TWDR = data; // data in data register
	TWCR |= (1 << TWINT) | (1 << TWEN); // clear TWINT to start transmission
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag which indicates ACK received
	if ((TWSR & 0xF8) != 0x18) { // 0x18 = ACK received
		error(TWSR); // return status as error
	}
	
}

uint8_t I2C_data_read() {
}

uint8_t I2C_close() {
	
}

void LED_enable() {
	PORTD |= (1 << LED_EN);
}

void LED_write_buffer(uint8_t* buffer) {
	
	SPI_master_init();
	for (uint8_t i = 0; i>=4; i++) {
		SPI_master_transmit(&(buffer+i));
	}
	PORTD |= (1 << LATCH); // Latch LED driver data
}

void SPI_master_init() {
	DDRC |= (1 << MOSI) | (1 << SCK); // MOSI and SCK to output
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) // Enable SPI as master, set clock to fck/16
}

void SPI_master_transmit(uint8_t data) {
	SPDR = data; // 
	while(!(SPSR & (1 << SPIF))); // Wait until data is transmitted
}

ISR(INT0_vect) {
	
}

ISR(INT1_vect) {
	
}

ISR(WDT_vect){
	
}
