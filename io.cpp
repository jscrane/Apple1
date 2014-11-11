#include <UTFT.h>
#include <memory.h>
#include <utftdisplay.h>

#include "io.h"
#include "config.h"
#include "hardware.h"

static byte dsp_cr, dsp, kbd_cr, kbd;
static unsigned kbd_int, dsp_out;

#define ROWS	24
#define COLS	40
static unsigned r, c;
static char screen[ROWS  * COLS];

void io::reset() {
	UTFTDisplay::begin(TFT_BG, TFT_FG);
	clear();

	dsp_cr = kbd_cr = 0;
	kbd_int = dsp_out = 0;
	r = c = 0;
}

// need ascii map for scan-codes
static const byte scanmap[] {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 	// 0x00
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0xff, 	// 0x08
	0xff, 0xff, 0xff, 0xff, 0xff, 0x51, 0x31, 0xff, 	// 0x10
	0xff, 0xff, 0x5a, 0x53, 0x41, 0x57, 0x32, 0xff, 	// 0x18
	0xff, 0x43, 0x58, 0x44, 0x45, 0x34, 0x33, 0xff, 	// 0x20
	0xff, 0x20, 0x56, 0x46, 0x54, 0x52, 0x35, 0xff, 	// 0x28
	0xff, 0x4e, 0x42, 0x48, 0x47, 0x59, 0x36, 0xff, 	// 0x30
	0xff, 0xff, 0x4d, 0x4a, 0x55, 0x37, 0x38, 0xff, 	// 0x38
	0xff, 0x2c, 0x4b, 0x49, 0x4f, 0x30, 0x39, 0xff, 	// 0x40
	0xff, 0x2e, 0x2f, 0x4c, 0x3b, 0x50, 0x2d, 0xff, 	// 0x48
	0xff, 0xff, 0x27, 0xff, 0x5b, 0x3d, 0xff, 0xff, 	// 0x50
	0xff, 0xff, 0x0d, 0x5d, 0xff, 0x23, 0xff, 0xff, 	// 0x58
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xff, 	// 0x60
	0xff, 0x31, 0xff, 0x34, 0x37, 0xff, 0xff, 0xff, 	// 0x68
	0x30, 0xff, 0x32, 0x35, 0x36, 0x38, 0xff, 0xff, 	// 0x70
	0xff, 0x2b, 0x33, 0x2d, 0x2a, 0x39, 0xff, 0xff, 	// 0x78
};

void io::down(unsigned scan) {
	kbd = 0;
}

void io::up(unsigned scan) {
	kbd = scanmap[scan] + 0x80;
	kbd_cr = 0xa7;
}

void io::display(byte b) {
	int x = c * _cx, y = r * _cy;
	char ch = (char)b;

	switch(b) {
	case 0x5f:
		if (--c < 0) {
			r--;
			c = COLS-1;
		}
		ch = ' ';
		utft.print(&ch, x, y);
		break;
	case 0x0a:
	case 0x0d:
		c = 0;
		r++;
		break;
	default:
		if (ch >= 0x20 && ch != 0x7f) {
			utft.print(&ch, x, y);
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
			for (int i = 0; i < COLS; i++) {
				int o = j*COLS + i, p = o + COLS;
				if (screen[p] != screen[o]) {
					screen[p] = screen[o];
					utft.print(&screen[p], i*_cx, j*_cy);
				}
			}
		for (int i = COLS; i-- > 0; ) {
			int o = ROWS*COLS - i;
			if (screen[o] != ' ') {
				screen[o] = ' ';
				utft.print(&screen[o], i*_cx, (ROWS-1)*_cy);
			}
		}
	}
}

void io::operator=(byte b) {

Serial.print(">");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(b, 16);

	switch(_acc % 4) {
	case 0:
		kbd = b;
		break;
	case 1:
		if (!kbd_int && b >= 0x80)
			kbd_int = 1;
		else
			kbd_cr = b;
		break;
	case 2:
		if (b >= 0x80)
			b -= 0x80;
		display(b);
		dsp = b;
		break;
	case 3:
		if (!dsp_out && dsp_cr >= 0x80)
			dsp_out = 1;
		else
			dsp_cr = b;
		break;
	}
}

io::operator byte() {

	switch (_acc % 4) {
	case 0:
Serial.print("<");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(kbd, 16);
		return kbd;
	case 1:
		if (kbd_int && kbd_cr >= 0x80) {
			kbd_cr = 0;
Serial.print("<");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(0xa7, 16);
			return 0xa7;
		}
//Serial.println(kbd_cr, 16);
		return kbd_cr;
	case 2:
Serial.print("<");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(dsp, 16);
		return dsp;
	case 3:
Serial.print("<");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(dsp_cr, 16);
		return dsp_cr;
	}
	return 0;
}

void io::checkpoint(Stream &s) {
}

void io::restore(Stream &s) {
}
