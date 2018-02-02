/******************************************************************************
 *  main.c: Source file for AVR thermostat
 *  See <http://willrobertson.id.au/> 
 *  Copyright (C) 2009 Will Robertson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int adc1, adc3, select;

int main() {
	wdt_disable();		// Kill the watchdog
	
	adc1 = 0;
	adc3 = 0;
	select = 0;

	DDRB |= 1;

	ADMUX = 0x21; 		// Vcc as voltage reference
				// Left align the data in ADCH
				// Select channel ADC1 for first conversion
	ADCSRA = 0x8F;		// Enable ADC, interrupt, and set prescaler to
				// 128 giving Fadc = 8MHz/128 = 62.5kHz

	sei();			// Enable interrupts

	ADCSRA |= (1 << ADSC);  // Start ADC Conversion 

	while(1) {
		sleep_mode(); 	// Put the AVR to sleep while waiting
				// for the ADC interrupt
	}
}

ISR (ADC_vect) {
	if(select) {
		adc3 = ADCH;
		ADMUX = 0x21;	// Select ADC1 for next conversion
		select = 0;
	} else {
		adc1 = ADCH;
		ADMUX = 0x23; 	// Select ADC2 for next conversion
		select = 1;
	}

	if(adc1 >= adc3) {
		if(adc1 - adc3 >= 2)	// Add a bit of hysteresis
			PORTB |= 0x01;
	} else {
		if(adc3 - adc1 >= 2)
			PORTB &= ~(0x01);
	}

	ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
}
