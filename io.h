#ifndef _IO_H
#define _IO_H

class serial_kbd;

class io: public Memory::Device, public Display, public PIA {
public:
	io(filer &files, serial_kbd &kbd): Memory::Device(Memory::page_size), files(files), _kbd(kbd) {}

	void reset();
	bool start();

	static void on_tick();

	void operator=(uint8_t b) { PIA::write(_acc, b); }
	operator uint8_t() { return PIA::read(_acc); }

	void checkpoint(Stream &);
	void restore(Stream &);

	void write_portb(uint8_t);
	uint8_t read_cra();

	void load();
	filer &files;

	static const uint8_t ROWS = 24;
	static const uint8_t COLS = 40;

private:
	serial_kbd &_kbd;

	void cursor(bool on);
	void display(uint8_t);
	void draw(char, int, int);
	void enter(uint8_t);

	bool _shift, _loading;
	uint8_t r, c;
	char screen[ROWS][COLS];
};

#endif
