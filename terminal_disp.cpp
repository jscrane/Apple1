#include <Arduino.h>
#include <serial_dsp.h>
#include "disp.h"
#include "terminal_disp.h"

void terminal_disp::status(const char *s) {
	_p.println(s);
}

void terminal_disp::write(uint8_t b) {
	char c = (char)b;
	switch(c) {
	case '_':
		_p.write(' ');
		_p.write('\b');
		_p.write('\b');
		_p.write('_');
		_p.write('\b');
		break;
	case '\r':
		_p.write(' ');
		_p.write('\r');
		_p.write('\n');
		break;
	default:
		_p.write(b);
		break;
	}
	_p.write('_');
	_p.write('\b');
}
