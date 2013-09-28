/*
 Heatit.cpp - Interrupt driven Heatit library for Arduino using 16 bit timer
 Copyright (c) 2013 Heatit Team.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 
 The Heatit Pin (H.) is activated by creating an instance of the Heatit class passing the desired pin to the setCurrent(mA) method.
 The Heatit Pins are controlled in the background using the value most recently written using the setCurrent(mA) method.
 
 Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
 
 The methods are:
 
 Heatit - Class for activating Heatit pins connected to Arduino pins.
 
 Heatit[].setCurrent()  - Attaches a servo motor to an i/o pin.
 attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
 default min is 544, max is 2400
 
 write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
 writeMicroseconds() - Sets the servo pulse width in microseconds
 read()      - Gets the last written servo pulse width as an angle between 0 and 180.
 readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
 attached()  - Returns true if there is a servo attached.
 detach()    - Stops an attached servos from pulsing its i/o pin.
 
 */

#include "Heatit.h"

#define POWERSTEP   16
#define LEDPIN_B    0x01
#define TXPIN_D     0x04

#define PWMPIN0_B   0x80
#define PWMPIN1_D   0x01
#define PWMPIN2_D   0x02
#define PWMPIN3_D   0x20
#define PWMPIN4_C   0x40
#define PWMPIN5_C   0x80
#define PWMPIN6_E   0x04
#define PWMPIN7_E   0x40

#define TA0_F   0x01
#define TA1_F   0x02
#define TA2_F   0x10
#define TA3_F   0x20


//extern const uint16_t PROGMEM patterns[Heatit_PINS *POWERSTEP];

uint8_t target[Heatit_PINS];

//const uint16_t patterns[Heatit_PINS *POWERSTEP] __ATTR_PROGMEM__ = {
extern const uint16_t PROGMEM patterns[Heatit_PINS *POWERSTEP]  = {
    //const uint16_t patterns[Heatit_PINS *POWERSTEP] = {
    0x0001, 0x0101, 0x0111, 0x1111, 0x1115, 0x1515, 0x1555, 0x5555, 0x5557, 0x5757, 0x5777, 0x7777, 0x777f, 0x7f7f, 0x7fff, 0xffff,
    0x0002, 0x0202, 0x0222, 0x2222, 0x222a, 0x2a2a, 0x2aaa, 0xaaaa, 0xaaae, 0xaeae, 0xaeee, 0xeeee, 0xeeef, 0xefef, 0xefff, 0xffff,
    0x0004, 0x0404, 0x0444, 0x4444, 0x4454, 0x4545, 0x4555, 0x5555, 0x5557, 0x5757, 0x5777, 0x7777, 0x777f, 0x7f7f, 0x7fff, 0xffff,
    0x0008, 0x0808, 0x0888, 0x8888, 0x88a8, 0x8a8a, 0x8aaa, 0xaaaa, 0xaaae, 0xaeae, 0xaeee, 0xeeee, 0xeeef, 0xefef, 0xefff, 0xffff,
    0x0010, 0x1010, 0x1110, 0x1111, 0x1511, 0x1515, 0x1555, 0x5555, 0x5557, 0x5757, 0x5777, 0x7777, 0x777f, 0x7f7f, 0x7fff, 0xffff,
    0x0020, 0x2020, 0x2220, 0x2222, 0x2a22, 0x2a2a, 0x2aaa, 0xaaaa, 0xaaae, 0xaeae, 0xaeee, 0xeeee, 0xeeef, 0xefef, 0xefff, 0xffff,
    0x0040, 0x4040, 0x4440, 0x4444, 0x5444, 0x4545, 0x4555, 0x5555, 0x5557, 0x5757, 0x5777, 0x7777, 0x777f, 0x7f7f, 0x7fff, 0xffff,
    0x0080, 0x8080, 0x8880, 0x8888, 0xa888, 0x8a8a, 0x8aaa, 0xaaaa, 0xaaae, 0xaeae, 0xaeee, 0xeeee, 0xeeef, 0xefef, 0xefff, 0xffff};

uint8_t pwmpattern_l[Heatit_PINS], pwmpattern_h[Heatit_PINS];
uint8_t phmask, phase_h;
uint8_t enabled;

#define ADMUX_MODE  0xe0
#define ADMUX_BITS  0x1f
#define ADMUX5_BIT  0x20

extern const uint8_t PROGMEM chmux[Heatit_PINS] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x07, 0x06};
extern const uint8_t PROGMEM chpatterns[Heatit_PINS]  = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//uint8_t chpatterns[Heatit_PINS] __ATTR_PROGMEM__ = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
//uint8_t chmux[Heatit_PINS] __ATTR_PROGMEM__ = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x07, 0x06};

uint8_t adphase;

void heat_init(){
    uint8_t  i;
    
    // set up heat pins
    for(i= 0; i < Heatit_PINS; i++){
        target[i]= 0;
        pwmpattern_l[i]= 0;
        pwmpattern_h[i]= 0;
    }
    enabled = 0;
    
    DDRB |= PWMPIN0_B;
    DDRD |= PWMPIN1_D;
    DDRD |= PWMPIN2_D;
    DDRD |= PWMPIN3_D;
    DDRC |= PWMPIN4_C;
    DDRC |= PWMPIN5_C;
    DDRE |= PWMPIN6_E;
    DDRE |= PWMPIN7_E;
    
    // Initializing the LED
    DDRB |= LEDPIN_B;
    PORTB |= LEDPIN_B;
    
#ifdef OSCTEST
    // Initializing the TX
    DDRD |= TXPIN_D;
    PORTD |= TXPIN_D;
#endif
    
    //phmask= 1;
    phase_h= 0;
    
    TCCR1A= 0x00;  TCCR1B= 0x0a;    // TCT mode  clk/2
    OCR1A= 2000;     //  IT
    TIMSK1= 0x02;   //OCIE1A
    
    // ADC init
    ADCSRA= 0xa3;
    DIDR2= 0x3f;
    ADMUX= ADMUX_MODE;
    ADCSRB= 0x00;
    ADCSRA= 0xe3;
    adphase= 0;
}

void heat_enable(uint8_t achan){
    if(target[achan])
        pwmpattern_l[achan]= pgm_read_byte(&patterns[achan<<4]);
    else
        pwmpattern_l[achan]= 0;
    pwmpattern_h[achan]= 0;
    enabled |= 1<<achan;
}

void heat_disable(uint8_t achan){
    pwmpattern_l[achan]= 0;
    pwmpattern_h[achan]= 0;
    enabled &= ~(1<<achan);
}

void heat_curset(uint8_t achan, uint16_t cur){
	target[achan]= (cur>>2)&0x7f;
	heat_enable(achan);
}

//Heatit current update
//TODO: convert it to assembly this part of code (now: 10us, would be 5-6us)
//a kulonbseg 5%ot vesz el a programidobol
SIGNAL(TIMER1_COMPA_vect){
    
    uint8_t tl, th;
    uint8_t n;
    uint16_t d;
    
    // tl= ADCL;
    // th= ADCH;
    tl = ADCH; //reading AD conversion from previous cycle
    
    if(enabled & phmask)
    {
        
        //3ff - 1.1V ---- 1.1A
        //protection from overcurrent
        //if((th & 0x03)||(tl & 0x80)){ //th felso byte (ha van adat, tularam), tl also byte
        if(tl & 0xc0){
            pwmpattern_l[adphase]= pwmpattern_h[adphase]= 0;
            enabled &= ~phmask;
        }
        //devision for 4 bits
        else if(phase_h)
        {
            th= target[adphase];
            n= 0;
            if(th >= tl){
                n= 0x08;
                th-= tl;
            }
            th<<= 1;
            if(th >= tl){
                n|= 0x04;
                th-= tl;
            }
            th<<= 1;
            if(th >= tl){
                n|= 0x02;
                th-= tl;
            }
            th<<= 1;
            if(th > tl) n++;
            n+= adphase<<4;
            // associate the calculated value to the table values
            d = pgm_read_word(&patterns[n]);
            pwmpattern_l[adphase]= d;
            pwmpattern_h[adphase]= d>>8;
        }
    }
    adphase= (adphase +1)& 0x7;
    //tl= pgm_read_byte(&chmux[adphase]);
    n = pgm_read_byte(&chmux[adphase]);
    // start the measurement on the next channel
    ADMUX= (n & ADMUX_BITS) | ADMUX_MODE; //multiplex allitas
    ADCSRB= n & ADMUX5_BIT; //AD inditas
    
    phmask = pgm_read_byte(&chpatterns[adphase]);
    if(phmask==1){
        phase_h ^= 1;
    }
    
    //setting current for the channels
    //
    
 	if(phase_h){
		if( pwmpattern_l[0]& phmask) PORTB |= PWMPIN0_B; else PORTB &= ~PWMPIN0_B;
		n= 0;
		if( pwmpattern_l[1]& phmask) n|= PWMPIN1_D;
		if( pwmpattern_l[2]& phmask) n|= PWMPIN2_D;
		if( pwmpattern_l[3]& phmask) n|= PWMPIN3_D;
		PORTD= (PORTD & ~(PWMPIN1_D |PWMPIN2_D |PWMPIN3_D)) |n;
        
		n= 0;
		if( pwmpattern_l[4]& phmask) n|= PWMPIN4_C;
		if( pwmpattern_l[5]& phmask) n|= PWMPIN5_C;
		PORTC= (PORTC  & ~(PWMPIN4_C |PWMPIN5_C)) |n;
        
		n= 0;
		if( pwmpattern_l[6]& phmask) n|= PWMPIN6_E;
		if( pwmpattern_l[7]& phmask) n|= PWMPIN7_E;
		PORTE= (PORTE  & ~(PWMPIN6_E |PWMPIN7_E)) |n;
	}
    else{
        if( pwmpattern_h[0]& phmask) PORTB |= PWMPIN0_B; else PORTB &= ~PWMPIN0_B;
        d= 0;
        if( pwmpattern_h[1]& phmask) d|= PWMPIN1_D;
        if( pwmpattern_h[2]& phmask) d|= PWMPIN2_D;
        if( pwmpattern_h[3]& phmask) d|= PWMPIN3_D;
        PORTD= (PORTD & ~(PWMPIN1_D |PWMPIN2_D |PWMPIN3_D)) |d;
        
        d= 0;
        if( pwmpattern_h[4]& phmask) d|= PWMPIN4_C;
        if( pwmpattern_h[5]& phmask) d|= PWMPIN5_C;
        PORTC= (PORTC  & ~(PWMPIN4_C |PWMPIN5_C)) |d;
        
        d= 0;
        if( pwmpattern_h[6]& phmask) d|= PWMPIN6_E;
        if( pwmpattern_h[7]& phmask) d|= PWMPIN7_E;
        PORTE= (PORTE  & ~(PWMPIN6_E |PWMPIN7_E)) |d;
    }
    
    //test
#ifdef OSCTEST
    PORTD &= ~TXPIN_D;
#endif
#ifdef OSCTEST
    PORTD |= TXPIN_D;
#endif
    
#ifdef ADTEST
    PORTD &= ~TXPIN_D;
    
    //    if(th & 0x02) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    //    if(th & 0x01) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    n = 0;
    
    if(n & 0x80) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x40) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x20) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x10) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x08) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x04) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x02) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    if(n & 0x01) PORTD |= TXPIN_D; else PORTD &= ~TXPIN_D;PORTD &= ~TXPIN_D;
    //PORTD |= TXPIN_D;
#endif
}

//---------------------------------------------------------------------------


void HeatitClass::setCurrentTarget(uint8_t pin, uint16_t _currentTarget){
    if (_currentTarget != 0) heat_curset(pin,  _currentTarget);
    else heat_disable(pin);
}

void HeatPin::setEnabled(bool _enabled) {
    if(_enabled){
        heat_enable(chn);
    }
    else{
        heat_disable(chn);
    }
}


void HeatPin::setCurrent(uint16_t _currentTarget){
    if (_currentTarget != 0) heat_curset(chn,  _currentTarget);
    else heat_disable(chn);
}

void HeatitClass::setup() {
    uint8_t  i;
    // set up heat pins
    for(i= 0; i < Heatit_PINS; i++){
        pins[i].chn= i;
    }
    
    heat_init();
}


void HeatitClass::led(int state) {
    if (state == HIGH) {
        PORTB &= ~LEDPIN_B;
    } else {
        PORTB |= LEDPIN_B;
    }
}

HeatitClass Heatit;
