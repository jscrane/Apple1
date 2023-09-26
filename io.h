#ifndef _IO_H
#define _IO_H

class io: public Memory::Device, public Display, Keyboard, public PIA {
public:
	io(filer &files): Memory::Device(Memory::page_size), files(files) {}

	virtual void reset();
	virtual void down(uint8_t scan);
	virtual void up(uint8_t scan);

	virtual void operator=(uint8_t b) { PIA::write(_acc, b); }
	virtual operator uint8_t() { return PIA::read(_acc); }

	virtual void checkpoint(Stream &);
	virtual void restore(Stream &);

	virtual void write_portb(uint8_t);
	virtual void write_portb_cr(uint8_t);
	virtual uint8_t read_porta_cr();
	virtual void write_porta_cr(uint8_t);

	void load();
	filer &files;
private:
	void display(uint8_t);
	void draw(char, int, int);
	void enter(uint8_t);

	bool _shift;
	bool _loading;
};

#endif
