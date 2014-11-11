#ifndef _IO_H
#define _IO_H

class io: public UTFTDisplay {
public:
	io(): UTFTDisplay(256) {}

	void operator=(byte);
	operator byte();

	void reset();
	void down(unsigned scan);
	void up(unsigned scan);

	void checkpoint(Stream &);
	void restore(Stream &);

private:
	void display(byte);

};

#endif
