#pragma once

class serial_kbd;
class disp;

class io: public Memory::Device, public Pollable {
public:
	io(filer &files, serial_kbd &kbd, disp &dsp): 
		Memory::Device(Memory::page_size), files(files), _kbd(kbd), _dsp(dsp) {}

	void reset();
	bool start();
	void poll();

	void operator=(uint8_t b) { _pia.write(_acc, b); }
	operator uint8_t() { return _pia.read(_acc); }

	void checkpoint(Checkpoint &);
	void restore(Checkpoint &);

	void load();
	filer &files;

private:
	PIA _pia;
	serial_kbd &_kbd;
	disp &_dsp;

	void enter(uint8_t);
	bool _loading;
	uint8_t _ch;
};
