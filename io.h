#ifndef _IO_H
#define _IO_H

class io: public UTFTDisplay, Keyboard {
public:
	io(): UTFTDisplay(256) {}

	void operator=(byte);
	operator byte();

	virtual void reset();
	virtual void down(byte scan);
	virtual void up(byte scan);

	void checkpoint(Stream &);
	void restore(Stream &);

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
