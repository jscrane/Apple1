#ifndef _IO_H
#define _IO_H

// http://mamedev.org/source/src/mess/machine/apple1.c.html
class io: public UTFTDisplay, Keyboard, public pia {
public:
	virtual void reset();
	virtual void down(byte scan);
	virtual void up(byte scan);

	virtual void checkpoint(Stream &);
	virtual void restore(Stream &);

	virtual void write_portb(byte);
	virtual byte read_porta_cr();

	void load();
	sdtape tape;
private:
	void display(byte);
	void draw(char, int, int);
	void enter(byte);

	bool _shift;
	bool _loading;
};

#endif
