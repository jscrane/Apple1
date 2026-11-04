#pragma once

#include <fnkey_kbd.h>
#include <filer.h>
class serial_dsp;
class PIA;

class io: public Pollable, private fnkey_kbd {
public:
	io(PIA &pia, filer &files, keyboard &kbd, serial_dsp &dsp):
		fnkey_kbd(kbd), files(files), _pia(pia), _dsp(dsp) {}

	void reset() override;
	void poll() override;
	using fnkey_kbd::register_fnkey_handler;

	bool start() { return files.start(); }
	void load();
	filer &files;

private:
	PIA &_pia;
	serial_dsp &_dsp;

	void enter(uint8_t);
	bool _loading;
	uint8_t _ch;
};
