#pragma once

class disp: public serial_dsp {
public:
	virtual void checkpoint(Checkpoint &) =0;
	virtual void restore(Checkpoint &) =0;

	virtual void status(const char *) =0;
};
