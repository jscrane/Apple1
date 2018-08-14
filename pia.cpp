#include <Arduino.h>
#include <memory.h>
#include "pia.h"

void pia::operator=(uint8_t b) {
/*
Serial.print(millis());
Serial.print(" > ");
Serial.print(_acc, 16);
Serial.print(' ');
Serial.println(b, 16);
*/
	switch(_acc % 4) {
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

pia::operator uint8_t() {
/*
Serial.print(millis());
Serial.print(" < ");
Serial.println(_acc, 16);
*/
	switch (_acc % 4) {
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

void pia::checkpoint(Stream &s) {
	s.write(portb_cr);
	s.write(portb);
	s.write(porta_cr);
	s.write(porta);
}

void pia::restore(Stream &s) {
	portb_cr = s.read();
	portb = s.read();
	porta_cr = s.read();
	porta = s.read();
}

