#ifndef _DISP_H
#define _DISP_H

class disp: public serial_dsp {
public:
	bool start();

	static void on_tick();

	virtual void checkpoint(Stream &) =0;
	virtual void restore(Stream &) =0;

	virtual void status(const char *) =0;

protected:
	virtual void cursor(bool on) =0;
};

#endif
