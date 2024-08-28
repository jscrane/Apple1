#include <Arduino.h>
#include <memory.h>
#include <display.h>
#include <serialio.h>
#include <filer.h>
#include <serial_kbd.h>
#include <serial_dsp.h>
#include <pia.h>
#include <timed.h>

#include "io.h"
#include "disp.h"
#include "hardware.h"
#include "config.h"

void io::reset() {
	_dsp.reset();
	_kbd.reset();

	_loading = false;
	PIA::reset();
}

bool io::start() {

	_dsp.start();
	return files.start();
}

void io::load() {
	if (files.more()) {
		_loading = true;
		enter(files.read());
	}
}

void io::enter(uint8_t key) {
	PIA::write_ca1(false);
	PIA::write_porta_in(key + 0x80);
	PIA::write_ca1(true);
}

void io::write_portb(uint8_t b) {
	b &= 0x7f;
	_dsp.write(b);
	PIA::write_portb(b);
}

uint8_t io::read_cra() {
	if (_loading) {
		if (files.more())
			enter(files.read());
		else
			_loading = false;
	} else if (_kbd.available()) {
		int c = _kbd.read();
		if (c != -1)
			enter(c);
	}

	return PIA::read_cra();
}

void io::checkpoint(Stream &s) {
	PIA::checkpoint(s);
	_dsp.checkpoint(s);
}

void io::restore(Stream &s) {
	PIA::restore(s);
	_dsp.restore(s);
}
