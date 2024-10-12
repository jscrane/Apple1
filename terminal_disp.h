#ifndef __TERMINAL_DISP_H__
#define __TERMINAL_DISP_H__

class terminal_disp: public disp {
public:
	terminal_disp(Print &p): _p(p) {}

	void checkpoint(Stream &) {}
	void restore(Stream &) {}
	void status(const char *);
	void write(uint8_t);

private:
	Print &_p;
};

#endif
