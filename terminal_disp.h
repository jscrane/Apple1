#pragma once

class terminal_disp: public disp {
public:
	terminal_disp(Print &p): _p(p) {}

	void checkpoint(Checkpoint &) {}
	void restore(Checkpoint &) {}
	void status(const char *);
	void write(uint8_t);

private:
	Print &_p;
};
