#ifndef __AVR_Heatit_h__
#define __AVR_Heatit_h

#include <stdio.h>
#include <Arduino.h>

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#define Heatit_PINS	8

class HeatitClass; // forward declaration

class HeatPin {
	private:
	uint8_t chn;

	public:
	void setEnabled(bool _enabled);
	void setCurrent(uint16_t _currentTarget);
	friend class HeatitClass;
};

class HeatitClass {
	public:
	void setup();
	void led(int state);
    void setCurrentTarget(uint8_t pin, uint16_t _currentTarget);
	HeatPin pins[Heatit_PINS];

};

extern HeatitClass Heatit;

#endif//Heatit_h