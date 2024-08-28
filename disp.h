#ifndef _DISP_H
#define _DISP_H

class disp: public serial_dsp {
public:
	virtual void checkpoint(Stream &) =0;
	virtual void restore(Stream &) =0;

	virtual void status(const char *) =0;
};

#endif
