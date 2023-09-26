#include <Arduino.h>
#include <memory.h>
#include "pia.h"

// see: https://github.com/mamedev/mame/blob/master/src/devices/machine/6821pia.cpp
// and: https://github.com/mamedev/mame/blob/master/src/devices/machine/6821pia.h

inline bool c1_low_to_high(uint8_t cr) { return cr & 0x02; }

inline bool c1_high_to_low(uint8_t cr) { return !c1_low_to_high(cr); }

void PIA::write(Memory::address a, uint8_t b) {
#if defined(DEBUGGING)
	Serial.print(millis());
	Serial.print(" > ");
	Serial.print(a, 16);
	Serial.print(' ');
	Serial.println(b, 16);
#endif
	switch(a % 4) {
	case 0:
		write_porta(b);
		break;
	case 1:
		write_porta_cr(b);
		break;
	case 2:
		write_portb(b);
		break;
	case 3:
		write_portb_cr(b);
		break;
	}
}

uint8_t PIA::read(Memory::address a) {
#if defined(DEBUGGING)
	Serial.print(millis());
	Serial.print(" < ");
	Serial.println(a, 16);
#endif
	switch (a % 4) {
	case 0:
		return read_porta();
	case 1:
		return read_porta_cr();
	case 2:
		return read_portb();
	case 3:
		return read_portb_cr();
	}
	return 0xff;
}

void PIA::checkpoint(Stream &s) {
	s.write(portb_cr);
	s.write(portb);
	s.write(porta_cr);
	s.write(porta);
	s.write(irq_b1);
	s.write(irq_a1);
	s.write(cb1);
	s.write(ca1);
}

void PIA::restore(Stream &s) {
	portb_cr = s.read();
	portb = s.read();
	porta_cr = s.read();
	porta = s.read();
	irq_b1 = s.read();
	irq_a1 = s.read();
	cb1 = s.read();
	ca1 = s.read();
}

void PIA::write_ca1(bool state) {

	if (ca1 == state)
		return;

	if ((state && c1_low_to_high(porta_cr)) || (!state && c1_high_to_low(porta_cr)))
		irq_a1 = true;

	ca1 = state;
}

void PIA::write_cb1(bool state) {

	if (cb1 == state)
		return;

	if ((state && c1_low_to_high(portb_cr)) || (!state && c1_high_to_low(portb_cr)))
		irq_b1 = true;

	cb1 = state;
}

uint8_t PIA::read_porta_cr() {
	if (irq_a1)
		return porta_cr | IRQ1;
	return porta_cr;
}

uint8_t PIA::read_portb_cr() {
	if (irq_b1)
		return portb_cr | IRQ1;
	return portb_cr;
}
