#include <Arduino.h>

#include <machine.h>
#include <memory.h>
#include <display.h>
#include <serial_dsp.h>
#include <hardware.h>
#include <debugging.h>

#include "disp.h"
#include "screen_disp.h"
#include "config.h"

#define CURSOR_BLINK	500000

void screen_disp::reset() {

	display.begin(BG_COLOUR, FG_COLOUR, ORIENT);
	display.setScreenChars(COLS, ROWS);
	display.clear();

	_machine->interval_timer(CURSOR_BLINK, [this]() {
		static int tick = 0;
		tick = (tick + 1) % 3;
		cursor(tick < 2);
	});

	r = c = 0;
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			screen[j][i] = ' ';
}

void screen_disp::cursor(bool on) {
	draw(on? '_': ' ', c, r);
}

void screen_disp::draw(char ch, int i, int j) {
	if (screen[j][i] != ch) {
		screen[j][i] = ch;
		display.setCursor(i * display.charWidth(), j * display.charHeight());
		display.print(ch);
	}
}

void screen_disp::write(uint8_t b) {
	char ch = (char)b;
	switch(ch) {
	case '_':
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

void screen_disp::checkpoint(Checkpoint &s) {
	s.write(r);
	s.write(c);
	for (int j = 0; j < ROWS; j++)
		for (int i = 0; i < COLS; i++)
			s.write(screen[j][i]);
}

void screen_disp::restore(Checkpoint &s) {
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
