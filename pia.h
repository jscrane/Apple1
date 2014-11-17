#ifndef __PIA_H__
#define __PIA_H__

class pia: public Memory::Device {
public:
	pia(): Memory::Device(256), portb_cr(0), porta_cr(0) {}
	virtual void reset() { portb_cr = porta_cr = 0; }

	void operator=(byte);
	operator byte();

	void checkpoint(Stream &);
	void restore(Stream &);

protected:
	// write to the "external" side of the port
	void set_porta(byte b) {
		porta = b;
		if (b & 0x80)
			porta_cr = 0xa7;
	}

	// "device-side" operations (called from memory interface)
	byte read_porta() { return porta; }
	virtual byte read_porta_cr() {
		if (porta_cr & 0x80) {
			porta_cr = 0;
			return 0xa7;
		}
		return porta_cr;
	}
	byte read_portb() { return portb; }
	byte read_portb_cr() { return portb_cr; }

	void write_porta(byte b) { porta = b; }
	void write_porta_cr(byte b) {
		if (!(porta_cr & 0x80) && b >= 0x80)
			porta_cr |= 0x80;
		else
			porta_cr = b;
	}
	virtual void write_portb(byte b) { portb = b; }
	void write_portb_cr(byte b) {
		if (portb_cr < 0x80)
			portb_cr = b;
	}

private:
	byte portb_cr, portb, porta_cr, porta;
};
#endif
