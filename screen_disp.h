#ifndef __SCREEN_DISP_H__
#define __SCREEN_DISP_H__

class screen_disp: public Display, public disp {
public:
	void reset();

	void checkpoint(Stream &);
	void restore(Stream &);

	void write(uint8_t);
	void status(const char *s) { Display::status(s); }

private:
	void cursor(bool on);
	void draw(char, int, int);

	static const uint8_t ROWS = 24;
	static const uint8_t COLS = 40;

	uint8_t r, c;
	char screen[ROWS][COLS];
};

#endif
