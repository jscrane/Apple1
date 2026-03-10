#pragma once

class serial_kbd;

class io: public Pollable {
public:
	io(PIA &pia, filer &files, serial_kbd &kbd, serial_dsp &dsp):
		files(files), _pia(pia), _kbd(kbd), _dsp(dsp) {}

	void reset();
	bool start() { return files.start(); }
	void poll();

	void load();
	filer &files;

private:
	PIA &_pia;
	serial_kbd &_kbd;
	serial_dsp &_dsp;

	void enter(uint8_t);
	bool _loading;
	uint8_t _ch;
};
