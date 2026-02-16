#include <Arduino.h>

#include <machine.h>
#include <memory.h>
#include <display.h>
#include <serialio.h>
#include <filer.h>
#include <serial_kbd.h>
#include <serial_dsp.h>
#include <pia.h>
#include <hardware.h>

#include "io.h"
#include "disp.h"
#include "config.h"

void io::reset() {
	_loading = false;
	_dsp.reset();
	_kbd.reset();
	_pia.reset();
	_ch = 0;
}

bool io::start() {

	_pia.register_portb_write_handler([this](uint8_t b) { _dsp.write(b & 0x7f); });
	_pia.register_porta_read_handler([this]() { uint8_t c = _ch; _ch = 0; return c; });

	return files.start();
}

void io::load() {
	if (files.more()) {
		_loading = true;
		enter(files.read());
	}
}

void io::enter(uint8_t key) {
	_pia.write_ca1(false);
	_ch = key | 0x80;
	_pia.write_ca1(true);
}

void io::poll() {
	if (_ch)
		return;

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
}

void io::checkpoint(Checkpoint &s) {
	_pia.checkpoint(s);
	_dsp.checkpoint(s);
}

void io::restore(Checkpoint &s) {
	_pia.restore(s);
	_dsp.restore(s);
}
