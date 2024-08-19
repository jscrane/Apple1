#include <Arduino.h>
#include <memory.h>
#include <display.h>
#include <serialio.h>
#include <filer.h>
#include <serial_kbd.h>
#include <pia.h>
#include <timed.h>

#include "io.h"
#include "hardware.h"
#include "config.h"

void io::reset() {
	Display::begin(BG_COLOUR, FG_COLOUR, ORIENT);
	clear();
	_cy += 2;

	r = c = 0;
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			screen[j][i] = ' ';

	_kbd.reset();

	_loading = false;
	PIA::reset();
}

static io *i;

const int TICK_FREQ = 2;

bool io::start() {

	i = this;
	timer_create(TICK_FREQ, io::on_tick);
	return files.start();
}

void IRAM_ATTR io::on_tick() {

	static int tick = 0;
	tick = ++tick % 3;
	i->cursor(tick < 2);
}

void io::cursor(bool on) {
	draw(on? '_': ' ', c, r);
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

void io::draw(char ch, int i, int j) {
	if (screen[j][i] != ch) {
		screen[j][i] = ch;
		char c[2] = { ch, 0 };
		drawString(c, i*_cx, j*_cy);
	}
}

void io::display(uint8_t b) {
	char ch = (char)b;
	switch(ch) {
	case 0x5f:
		draw(' ', c, r);
		if (c-- == 0) {
			r--;
			c = COLS-1;
		}
		break;
	case 0x0d:
		draw(' ', c, r);
		c = 0;
		r++;
		break;
	default:
		if (ch >= 0x20 && ch < 0x7f) {
			draw(ch, c, r);
			if (++c == COLS) {
				c = 0;
				r++;
			}
		}
	}
	if (r == ROWS) {
		// scroll
		r--;
		for (int j = 0; j < (ROWS-1); j++)
			for (int i = 0; i < COLS; i++)
				draw(screen[j+1][i], i, j);
		for (int i = 0; i < COLS; i++)
			draw(' ', i, ROWS-1);
	}
	draw('_', c, r);
}

void io::write_portb(uint8_t b) {
	b &= 0x7f;
	display(b);
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
	s.write(r);
	s.write(c);
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			s.write(screen[j][i]);
}

void io::restore(Stream &s) {
	PIA::restore(s);
	r = s.read();
	c = s.read();
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++) {
			char c = s.read();
			screen[j][i] = c;
			draw(c, i, j);
		}
	draw('_', c, r);
}
