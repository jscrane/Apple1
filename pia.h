#ifndef __PIA_H__
#define __PIA_H__

// https://en.wikipedia.org/wiki/Peripheral_Interface_Adapter
class PIA {
public:
	PIA(): portb(0), portb_cr(0), porta(0), porta_cr(0) {}
	virtual void reset() { portb = portb_cr = porta = porta_cr = 0; }

	void write(Memory::address, uint8_t);
	uint8_t read(Memory::address);

	void checkpoint(Stream &);
	void restore(Stream &);

protected:
	// "device-side" operations (called from memory interface)
	virtual uint8_t read_porta() { return porta; }
	virtual uint8_t read_porta_cr() { return porta_cr; }
	virtual uint8_t read_portb() { return portb; }
	virtual uint8_t read_portb_cr() { return portb_cr; }

	virtual void write_porta(uint8_t b) { porta = b; }
	virtual void write_porta_cr(uint8_t b) { porta_cr = b; }
	virtual void write_portb(uint8_t b) { portb = b; }
	virtual void write_portb_cr(uint8_t b) { portb_cr = b; }

private:
	uint8_t portb_cr, portb, porta_cr, porta;
};
#endif
