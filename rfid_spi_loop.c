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

// WARNING! If you misconfigure port B, you will fry your Pi!
#define DD_SS   PB2
#define DD_MOSI PB3
#define DD_MISO PB4
#define DD_SCK  PB5
#define DD_SPI_PORT PORTB
#define DD_SPI_CONTROL DDRB

#define NOTHING 0xFF


void wait_for_next() {
	for(;;) {
		if(SPSR & (1 << SPIF))
			return;
	}
}

void tx_data(uint32_t data) {
	for (int i = 0; i < 4; i++) {
		SPDR = data;
		data >>= 8;
		wait_for_next();
	}
}

int rfid_spi_loop(void) {

	// enable data port as input
	DATA_PORT_CONTROL = 0x00;

	// enable led port as output
	DEBUG_LED_CONTROL = 0xff;

	// set MISO output, all others input, to avoid frying the Raspberry PI
	DD_SPI_CONTROL = (1 << DD_MISO);

	// enable SPI -- this enables PINs on PORTB as SPI MISO, MOSI, etc
	SPCR = (1 << SPE);

	uint8_t out_pending = 0;
	uint32_t data = 0x00;
	uint8_t count = 0;
	uint8_t t;

	DEBUG_LED_OUT = 0xf8;
	SPDR = NOTHING;

	for(;;) {
		t = DATA_PORT_IN;
		if(bit_is_clear(t, DATA0)) {
			// input is a Wiegand '0'
			data <<= 1;
			count++;

			while(bit_is_clear(DATA_PORT_IN, DATA0)) {}
		}
		else if(bit_is_clear(t, DATA1)) {
			// input is a Wiegand '1'
			data <<= 1;
			data |= 1;
			count++;

			while(bit_is_clear(DATA_PORT_IN, DATA1)) {}
		}

		if(count >= 26) {
			out_pending = 1;
			count = 0;
			DEBUG_LED_OUT = ~data;
		}

		/* Wait for SPI reception complete */
		if(SPSR & (1 << SPIF)) {
			if(out_pending) {
				tx_data(data);
				data = 0;
				out_pending = 0;
			} else {
				SPDR = NOTHING;
			}
		}
	}
	return 0;
}
