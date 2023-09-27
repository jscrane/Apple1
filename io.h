#ifndef _IO_H
#define _IO_H

class io: public Memory::Device, public Display, Keyboard, public PIA {
public:
	io(filer &files): Memory::Device(Memory::page_size), files(files) {}

	virtual void reset();
	virtual void down(uint8_t);
	virtual void up(uint8_t);

	virtual void operator=(uint8_t b) { PIA::write(_acc, b); }
	virtual operator uint8_t() { return PIA::read(_acc); }

	virtual void checkpoint(Stream &);
	virtual void restore(Stream &);

	virtual void write_portb(uint8_t);
	virtual uint8_t read_cra();

	void load();
	filer &files;

	static const uint8_t ROWS = 24;
	static const uint8_t COLS = 40;

private:
	void display(uint8_t);
	void draw(char, int, int);
	void enter(uint8_t);

	bool _shift, _loading;
	uint8_t r, c;
	char screen[ROWS][COLS];
};

#endif
