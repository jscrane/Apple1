#ifndef _DISP_H
#define _DISP_H

class serial_kbd;

class disp: public Display {
public:
	void reset();
	bool start();

	static void on_tick();

	void checkpoint(Stream &);
	void restore(Stream &);

	static const uint8_t ROWS = 24;
	static const uint8_t COLS = 40;

	void display(uint8_t);
	void cursor(bool on);
private:
	void draw(char, int, int);

	uint8_t r, c;
	char screen[ROWS][COLS];
};

#endif
