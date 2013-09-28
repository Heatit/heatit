/*
  pins_arduino.h - Pin definition functions for Heatit
  Heatit -- http://heatit.cc
 
  Modified from Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received   a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Workaround for wrong definitions in "iom32u4.h".
// This should be fixed in the AVR toolchain.
#undef UHCON
#undef UHINT
#undef UHIEN
#undef UHADDR
#undef UHFNUM
#undef UHFNUML
#undef UHFNUMH
#undef UHFLEN
#undef UPINRQX
#undef UPINTX
#undef UPNUM
#undef UPRST
#undef UPCONX
#undef UPCFG0X
#undef UPCFG1X
#undef UPSTAX
#undef UPCFG2X
#undef UPIENX
#undef UPDATX
#undef TCCR2A
#undef WGM20
#undef WGM21
#undef COM2B0
#undef COM2B1
#undef COM2A0
#undef COM2A1
#undef TCCR2B
#undef CS20
#undef CS21
#undef CS22
#undef WGM22
#undef FOC2B
#undef FOC2A
#undef TCNT2
#undef TCNT2_0
#undef TCNT2_1
#undef TCNT2_2
#undef TCNT2_3
#undef TCNT2_4
#undef TCNT2_5
#undef TCNT2_6
#undef TCNT2_7
#undef OCR2A
#undef OCR2_0
#undef OCR2_1
#undef OCR2_2
#undef OCR2_3
#undef OCR2_4
#undef OCR2_5
#undef OCR2_6
#undef OCR2_7
#undef OCR2B
#undef OCR2_0
#undef OCR2_1
#undef OCR2_2
#undef OCR2_3
#undef OCR2_4
#undef OCR2_5
#undef OCR2_6
#undef OCR2_7


//Heatit uncommented
#define NUM_DIGITAL_PINS  6
#define NUM_ANALOG_INPUTS 4

#define TX_RX_LED_INIT	DDRD |= (1<<3), DDRD |= (1<<2)
#define TXLED0			PORTD |= (1<<3)
#define TXLED1			PORTD &= ~(1<<3)
#define RXLED0			PORTD |= (1<<2)
#define RXLED1			PORTD &= ~(1<<2)

//mapping Heatit pins:

//static const uint8_t SDA = 2;
//static const uint8_t SCL = 3;


//Heatit specific 

// Mapping Heatit digital pins as digital I/O
const static uint8_t D0 = 0; //RX
const static uint8_t RX = 0; //RX
const static uint8_t D1 = 1;  //TX
const static uint8_t TX = 1;  //TX

// Mapping Heatit analog pins as digital I/O
const static uint8_t A0 = 2;
const static uint8_t A1 = 3;
const static uint8_t A2 = 4;
const static uint8_t A3 = 5;


// Map SPI port to 'new' pins D14..D17
static const uint8_t SS   = 17;
static const uint8_t MOSI = 16;
static const uint8_t MISO = 14;
static const uint8_t SCK  = 15;


/*// Heatit pins
static const uint8_t H0 = 12;
static const uint8_t H1 = 18;
static const uint8_t H2 = 19;
static const uint8_t H3 = 22;
static const uint8_t H4 = 31;
static const uint8_t H5 = 32;
static const uint8_t H6 = 33;
static const uint8_t H7 = 1;

*/



#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 2) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 3) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 3) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 3) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))
/*
#define digitalPinToPCICR(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCMSK0) : ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) ( ((p) >= 8 && (p) <= 11) ? (p) - 4 : ((p) == 14 ? 3 : ((p) == 15 ? 1 : ((p) == 16 ? 2 : ((p) == 17 ? 0 : (p - A8 + 4))))))*/

//	__AVR_ATmega32U4__ has an unusual mapping of pins to channels
extern const uint8_t PROGMEM analog_pin_to_channel_PGM[];
#define analogPinToChannel(P)  ( pgm_read_byte( analog_pin_to_channel_PGM + (P) ) )

#ifdef ARDUINO_MAIN


// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA32U4 / Heatit Pins

// D0        PD2                 RXD0/INT2
// D1        PD3                 TXD1/INT3

// D2  A0    PF0                 ADC0           
// D3  A1    PF1                 ADC1
// D4  A2    PF4        ADC4     TCK
// D5  A3    PF5        ADC5     TMS

// MISO		D14		PB3					MISO,PCINT3
// SCK		D15		PB1					SCK,PCINT1
// MOSI		D16		PB2					MOSI,PCINT2
// SS		D17		PB0					RXLED,SS/PCINT0

// D10  H0   PB7                 OC0A/OC1C/PCINT7/#RTS
// D11  H1   PD0                 OC0B/SCL/INT0
// D12  H2   PD1                 SDA/INT1
// D13  H3   PD5                 XCK1/#CTS
// D14  H4   PC6                 OC3A/#OC4A
// D15  H5   PC7                 OC4A/CLK0/ICP3
// D16  H6   PE2                 #HWB
// D17  H7   PE6                 INT.6/AIN0

//Heatit FB Feedbacks
// A10 HFH7  ADC6
// A11 HFH6  ADC7
// A5  HFB0  ADC8
// A4  HFB1  ADC9
// A6  HFB2  ADC10
// A7  HFB3  ADC11
// A8  HFB4  ADC12
// A9  HFB5  ADC13


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
	(uint16_t) &DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
	(uint16_t) &PINF,
};


// Heatit specific
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	
    PD, // D0 - PD2 - RX
	PD,	// D1 - PD3 - TX
        
	PF, // D2 - PF0 - A0
	PF,	// D3 - PF1 - A1
	PF,	// D4 - PF4 - A2
	PF, // D5 - PF5 - A3
    
	PB,	// D14 - MISO - PB3
	PB,	// D15 - SCK - PB1
	PB,	// D16 - MOSI - PB2
	PB,	// D17 - SS - PB0
    
    /* defined in heatit.cpp
	PB, // D10 - PB7 - H0
	PD,	// D11 - PD0 - H1
	PD, // D12 - PD1 - H2
	PD, // D13 - PD5 - H3
    PC, // D14 - PC6 - H4
    PC,	// D15 - PC7 - H5
	PE,	// D16 - PE2 - H6
	PE,	// D17 - PE6 - H7*/
};


// Heatit specific
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	
    _BV(2), // D0 - PD2 - RX
	_BV(3),	// D1 - PD3 - TX
    
	_BV(0), // D2 - PF0 - A0
	_BV(1),	// D3 - PF1 - A1
	_BV(4),	// D4 - PF4 - A2
	_BV(5), // D5 - PF5 - A3
    
	_BV(3),	// D14 - MISO - PB3
	_BV(1),	// D15 - SCK - PB1
	_BV(2),	// D16 - MOSI - PB2
	_BV(0),	// D17 - SS - PB0
    
    /* defined in heatit.cpp
	_BV(7), // D10 - PB7 - H0
	_BV(0),	// D11 - PD0 - H1
	_BV(1), // D12 - PD1 - H2
	_BV(5), // D13 - PD5 - H3
	_BV(6),	// D14 - PC6 - H4
	_BV(7),	// D15 - PC7 - H5
	_BV(2),	// D16 - PE2 - H6
	_BV(6),	// D17 - PE6 - H7 */
	
};

//Heatit Specific -- no pwm port
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	
    NOT_ON_TIMER,
	NOT_ON_TIMER,
	
    NOT_ON_TIMER,
    NOT_ON_TIMER,
    NOT_ON_TIMER,
    NOT_ON_TIMER,
    
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	
	
};


//Heatit specific
const uint8_t PROGMEM analog_pin_to_channel_PGM[] = {
	2,	// A0				PF0					ADC0
	3,	// A1				PF1					ADC1
	4,	// A2				PF4					ADC4
	5,	// A3				PF5					ADC5
    
    //the followings are Heatit FB pins
	/*
    9,	// A4  HFB1  ADC9
	8,	// A5  HFB0  ADC8
	10,	// A6  HFB2  ADC10
	11,	// A7  HFB3  ADC11
	12,	// A8  HFB4  ADC12
	13,	// A9  HFB5  ADC13
	6,	// A10 HFH7  ADC6
	7,	// A11 HFH6  ADC7 */
    
};

#endif /* ARDUINO_MAIN */
#endif /* Pins_Arduino_h */
