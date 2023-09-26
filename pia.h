#ifndef __PIA_H__
#define __PIA_H__

// https://en.wikipedia.org/wiki/Peripheral_Interface_Adapter
class PIA {
public:
	PIA(): portb(0), portb_cr(0), porta(0), porta_cr(0), irq_a1(false), irq_b1(false) {}

	virtual void reset() {
		portb = portb_cr = porta = porta_cr = 0;
		irq_a1 = irq_b1 = ca1 = cb1 = false;
	}

	void write(Memory::address, uint8_t);
	uint8_t read(Memory::address);

	void checkpoint(Stream &);
	void restore(Stream &);

	void write_ca1(bool);
	void write_cb1(bool);

	static const uint8_t IRQ1 = 0x80;

protected:
	// "device-side" operations (called from memory interface)
	virtual uint8_t read_porta() { irq_a1 = false; return porta; }
	virtual uint8_t read_porta_cr();
	virtual uint8_t read_portb() { irq_b1 = false; return portb; }
	virtual uint8_t read_portb_cr();

	virtual void write_porta(uint8_t b) { porta = b; }
	virtual void write_porta_cr(uint8_t b) { porta_cr = (b & 0x3f); }
	virtual void write_portb(uint8_t b) { portb = b; }
	virtual void write_portb_cr(uint8_t b) { portb_cr = (b & 0x3f); }

private:
	uint8_t portb_cr, portb, porta_cr, porta;
	bool ca1, irq_a1;
	bool cb1, irq_b1;
};
#endif
