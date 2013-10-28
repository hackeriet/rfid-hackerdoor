#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>

// use port D as output (for LEDs)
#define DEBUG_LED_OUT PORTD
#define DEBUG_LED_CONTROL DDRD

// use port C as input
#define DATA_PORT_IN PINC
#define DATA_PORT_CONTROL DDRC
#define DATA0  PC0
#define DATA1  PC1
#define LED    PC2
#define BUZZER PC3

int rfid_main(void) {

	// enable data port as input
	DATA_PORT_CONTROL = 0x00;

	// enable led port as output
	DEBUG_LED_CONTROL = 0xff;

	uint32_t data = 0x00;
	uint8_t count = 0x00;

	DEBUG_LED_OUT = 0xff;

	for(;;) {
		if(bit_is_clear(DATA_PORT_IN, DATA0)) {
			// input is a Wiegand '0'
			data <<= 1;
			count++;

			while(bit_is_clear(DATA_PORT_IN, DATA0)) {}
		}
		else if(bit_is_clear(DATA_PORT_IN, DATA1)) {
			// input is a Wiegand '1'
			data <<= 1;
			data |= 1;
			count++;

			while(bit_is_clear(DATA_PORT_IN, DATA1)) {}
		}
		DEBUG_LED_OUT = ~data;
	}

	return 0;
}
