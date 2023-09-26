#include <Arduino.h>
#include <memory.h>
#include "pia.h"

// see: https://github.com/mamedev/mame/blob/master/src/devices/machine/6821pia.cpp
// and: https://github.com/mamedev/mame/blob/master/src/devices/machine/6821pia.h

inline bool c1_low_to_high(uint8_t cr) { return cr & 0x02; }

inline bool c1_high_to_low(uint8_t cr) { return !c1_low_to_high(cr); }

inline bool c2_output(uint8_t cr) { return cr & 0x20; }

inline bool c2_input(uint8_t cr) { return !c2_input(cr); }

inline bool c2_low_to_high(uint8_t cr) { return cr & 0x10; }

inline bool c2_high_to_low(uint8_t cr) { return !c2_low_to_high(cr); }

inline bool output_selected(uint8_t cr) { return cr & 0x04; }

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
		output_selected(porta_cr)? write_porta(b): write_ddra(b);
		break;
	case 1:
		write_porta_cr(b);
		break;
	case 2:
		output_selected(portb_cr)? write_portb(b): write_ddrb(b);
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
		return output_selected(porta_cr)? read_porta(): read_ddra();
	case 1:
		return read_porta_cr();
	case 2:
		return output_selected(portb_cr)? read_portb(): read_ddrb();
	case 3:
		return read_portb_cr();
	}
	return 0xff;
}

void PIA::checkpoint(Stream &s) {
	s.write(portb_cr);
	s.write(portb);
	s.write(ddrb);
	s.write(porta_cr);
	s.write(porta);
	s.write(ddra);
	s.write(irq_b1);
	s.write(irq_b2);
	s.write(irq_a1);
	s.write(irq_a2);
	s.write(cb1);
	s.write(cb2);
	s.write(ca1);
	s.write(ca2);
}

void PIA::restore(Stream &s) {
	portb_cr = s.read();
	portb = s.read();
	ddrb = s.read();
	porta_cr = s.read();
	porta = s.read();
	ddra = s.read();
	irq_b1 = s.read();
	irq_b2 = s.read();
	irq_a1 = s.read();
	irq_a2 = s.read();
	cb1 = s.read();
	cb2 = s.read();
	ca1 = s.read();
	ca2 = s.read();
}

void PIA::write_ca1(bool state) {

	if (ca1 == state)
		return;

	if ((state && c1_low_to_high(porta_cr)) || (!state && c1_high_to_low(porta_cr)))
		irq_a1 = true;

	ca1 = state;
}

void PIA::write_ca2(bool state) {

	if (ca2 == state || !c2_input(porta_cr))
		return;

	if ((state && c2_low_to_high(porta_cr)) || (!state && c2_high_to_low(porta_cr)))
		irq_a2 = true;

	ca2 = state;
}

void PIA::write_cb1(bool state) {

	if (cb1 == state)
		return;

	if ((state && c1_low_to_high(portb_cr)) || (!state && c1_high_to_low(portb_cr)))
		irq_b1 = true;

	cb1 = state;
}

void PIA::write_cb2(bool state) {

	if (cb2 == state || !c2_input(portb_cr))
		return;

	if ((state && c2_low_to_high(portb_cr)) || (!state && c2_high_to_low(portb_cr)))
		irq_b2 = true;

	cb2 = state;
}

uint8_t PIA::read_porta_cr() {
	byte b = porta_cr;

	if (irq_a1)
		b |= IRQ1;

	if (irq_a2 && c2_input(porta_cr))
		b |= IRQ2;

	return b;
}

uint8_t PIA::read_portb_cr() {
	byte b = portb_cr;

	if (irq_b1)
		b |= IRQ1;

	if (irq_b2 && c2_input(portb_cr))
		b |= IRQ2;

	return b;
}
