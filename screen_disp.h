#pragma once

class Display;

class screen_disp: public serial_dsp {
public:
	screen_disp(Display &d): display(d) {}

	void reset();

	void checkpoint(Checkpoint &);
	void restore(Checkpoint &);

	void write(uint8_t);
	void status(const char *);

private:
	Display &display;

	void cursor(bool on);
	void draw(char, int, int);

	static const uint8_t ROWS = 24;
	static const uint8_t COLS = 40;

	uint8_t r, c;
	char screen[ROWS][COLS];
};
