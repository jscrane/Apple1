#ifndef _IO_H
#define _IO_H

class serial_kbd;
class disp;

class io: public Memory::Device, public PIA {
public:
	io(filer &files, serial_kbd &kbd, disp &dsp): 
		Memory::Device(Memory::page_size), files(files), _kbd(kbd), _dsp(dsp) {}

	void reset();
	bool start();

	void operator=(uint8_t b) { PIA::write(_acc, b); }
	operator uint8_t() { return PIA::read(_acc); }

	void checkpoint(Stream &);
	void restore(Stream &);

	void write_portb(uint8_t);
	uint8_t read_cra();

	void load();
	filer &files;

private:
	serial_kbd &_kbd;
	disp &_dsp;

	void enter(uint8_t);
	bool _loading;
};

#endif
