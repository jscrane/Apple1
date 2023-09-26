#ifndef __PIA_H__
#define __PIA_H__

// https://en.wikipedia.org/wiki/Peripheral_Interface_Adapter
class PIA {
public:
	PIA(): portb(0), portb_cr(0), porta(0), porta_cr(0),
		ca1(false), ca2(false), cb1(false), cb2(false),
		irq_a1(false), irq_a2(false), irq_b1(false), irq_b2(false) {}

	virtual void reset() {
		portb = portb_cr = porta = porta_cr = 0;
		irq_a1 = irq_a2 = irq_b1 = irq_b2 = ca1 = ca2 = cb1 = cb2 = false;
	}

	void write(Memory::address, uint8_t);
	uint8_t read(Memory::address);

	void checkpoint(Stream &);
	void restore(Stream &);

	void write_ca1(bool);
	void write_ca2(bool);
	void write_cb1(bool);
	void write_cb2(bool);

	static const uint8_t IRQ1 = 0x80;
	static const uint8_t IRQ2 = 0x40;

protected:
	// "device-side" operations (called from memory interface)
	virtual uint8_t read_porta() { irq_a1 = irq_a2 = false; return porta; }
	virtual uint8_t read_porta_cr();
	virtual uint8_t read_portb() { irq_b1 = irq_b2 = false; return portb; }
	virtual uint8_t read_portb_cr();

	virtual void write_porta(uint8_t b) { porta = b; }
	virtual void write_porta_cr(uint8_t b) { porta_cr = (b & 0x3f); }
	virtual void write_portb(uint8_t b) { portb = b; }
	virtual void write_portb_cr(uint8_t b) { portb_cr = (b & 0x3f); }

private:
	uint8_t portb_cr, portb, porta_cr, porta;
	bool ca1, ca2, irq_a1, irq_a2;
	bool cb1, cb2, irq_b1, irq_b2;
};
#endif
