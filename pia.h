#ifndef __PIA_H__
#define __PIA_H__

// https://en.wikipedia.org/wiki/Peripheral_Interface_Adapter
class PIA {
public:
	PIA(): portb_cr(0), porta_cr(0) {}
	virtual void reset() { portb_cr = porta_cr = 0; }

	void write(Memory::address, uint8_t);
	uint8_t read(Memory::address);

	void checkpoint(Stream &);
	void restore(Stream &);

protected:
	// write to the "external" side of the port
	void set_porta(uint8_t b) {
		porta = b;
		if (b & 0x80)
			porta_cr = 0xa7;
	}

	// "device-side" operations (called from memory interface)
	uint8_t read_porta() { return porta; }
	virtual uint8_t read_porta_cr() {
		if (porta_cr & 0x80) {
			porta_cr = 0;
			return 0xa7;
		}
		return porta_cr;
	}
	uint8_t read_portb() { return portb; }
	uint8_t read_portb_cr() { return portb_cr; }

	void write_porta(uint8_t b) { porta = b; }
	void write_porta_cr(uint8_t b) {
		if (!(porta_cr & 0x80) && b >= 0x80)
			porta_cr |= 0x80;
		else
			porta_cr = b;
	}
	virtual void write_portb(uint8_t b) { portb = b; }
	void write_portb_cr(uint8_t b) {
		if (portb_cr < 0x80)
			portb_cr = b;
	}

private:
	uint8_t portb_cr, portb, porta_cr, porta;
};
#endif
