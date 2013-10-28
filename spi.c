#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DD_MISO PB4
#define DDR_SPI DDRB

int spi_main() {
	// Set PD to output
	DDRD = 0xff;
	PORTD = 0x0f;

	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);

	for(;;) {
		/* Wait for reception complete */
		if(SPSR & (1<<SPIF)) {
			SPDR = 0x55;
			PORTD = 0xf0;
		}
	}
	return 0;
}
