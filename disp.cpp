#include <Arduino.h>
#include <memory.h>
#include <display.h>
#include <serialio.h>
#include <filer.h>
#include <serial_kbd.h>
#include <pia.h>
#include <timed.h>

#include "disp.h"
#include "hardware.h"
#include "config.h"

void disp::reset() {
	Display::begin(BG_COLOUR, FG_COLOUR, ORIENT);
	Display::clear();
	_cy += 2;

	r = c = 0;
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			screen[j][i] = ' ';
}

static disp *i;

const int TICK_FREQ = 2;

bool disp::start() {

	i = this;
	timer_create(TICK_FREQ, disp::on_tick);
	return true;
}

void IRAM_ATTR disp::on_tick() {

	static int tick = 0;
	tick = ++tick % 3;
	i->cursor(tick < 2);
}

void disp::cursor(bool on) {
	draw(on? '_': ' ', c, r);
}

void disp::draw(char ch, int i, int j) {
	if (screen[j][i] != ch) {
		screen[j][i] = ch;
		char c[2] = { ch, 0 };
		drawString(c, i*_cx, j*_cy);
	}
}

void disp::display(uint8_t b) {
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

void disp::checkpoint(Stream &s) {
	s.write(r);
	s.write(c);
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			s.write(screen[j][i]);
}

void disp::restore(Stream &s) {
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
