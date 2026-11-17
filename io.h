#pragma once

#include <fnkey_kbd.h>
class serial_dsp;
class PIA;
class flash_file;

class io: public Pollable, private fnkey_kbd {
public:
	io(PIA &pia, flash_file &tape, keyboard &kbd, serial_dsp &dsp):
		fnkey_kbd(kbd), _tape(tape), _pia(pia), _dsp(dsp) {}

	void reset() override;
	void poll() override;
	using fnkey_kbd::register_fnkey_handler;

	void load();

private:
	flash_file &_tape;
	PIA &_pia;
	serial_dsp &_dsp;

	void enter(uint8_t);
	bool _loading;
	uint8_t _ch;
};
