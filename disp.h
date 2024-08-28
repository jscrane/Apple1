#ifndef _DISP_H
#define _DISP_H

class serial_kbd;

class disp {
public:
	virtual void reset() =0;
	bool start();

	static void on_tick();

	virtual void checkpoint(Stream &) =0;
	virtual void restore(Stream &) =0;

	virtual void display(uint8_t) =0;

protected:
	virtual void cursor(bool on) =0;
};

#endif
