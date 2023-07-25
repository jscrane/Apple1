#ifndef _IO_H
#define _IO_H

// http://mamedev.org/source/src/mess/machine/apple1.c.html
class io: public Display, Keyboard, public pia {
public:
	io(filer &files): files(files) {}

	virtual void reset();
	virtual void down(uint8_t scan);
	virtual void up(uint8_t scan);

	virtual void checkpoint(Stream &);
	virtual void restore(Stream &);

	virtual void write_portb(uint8_t);
	virtual uint8_t read_porta_cr();

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
