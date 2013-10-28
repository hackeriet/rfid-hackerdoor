#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>

/* WARNING! If you misconfigure port B, you will fry your Pi! */

#define DD_SS   PB2
#define DD_MOSI PB3
#define DD_MISO PB4
#define DD_SCK  PB5
#define DD_SPI_PORT PORTB
#define DD_SPI_CONTROL DDRB

// use port D as output (for LEDs)
#define DEBUG_LED_OUT PORTD
#define DEBUG_LED_CONTROL DDRD

int spi_main() {
	// enable LED port as output
	DEBUG_LED_CONTROL = 0xff;

	DEBUG_LED_OUT = 0x08;

	/* Set MISO output, all others input, to avoid frying the Raspberry PI */
	DD_SPI_CONTROL = (1 << DD_MISO);

	/* Enable SPI -- this enables PINs on PORTB as SPI MISO, MOSI, etc */
	SPCR = (1 << SPE);

	for(;;) {
		/* Wait for SPI reception complete */
		if(SPSR & (1 << SPIF)) {
			SPDR = 0x55;
			DEBUG_LED_OUT = 0xf0;
		}
	}
	return 0;
}
