#pragma once

class terminal_disp: public serial_dsp {
public:
	terminal_disp(Print &p): _p(p) {}

	void status(const char *);
	void write(uint8_t);

private:
	Print &_p;
};
