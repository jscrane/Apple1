#include <stdarg.h>
#include <SPI.h>
#include <SD.h>
#include <SpiRAM.h>
#include <UTFT.h>
#include <r65emu.h>

#include "roms/basic.h"
#include "roms/monitor.h"
#include "pia.h"
#include "io.h"
#include "config.h"

prom b(basic, sizeof(basic));
prom m(monitor, sizeof(monitor));
ram pages[RAM_SIZE / 1024];
io io;

void status(const char *fmt, ...) {
	char tmp[256];  
	va_list args;
	va_start(args, fmt);
	vsnprintf(tmp, sizeof(tmp), fmt, args);
	Serial.println(tmp);
	io.clear();
	io.error(tmp);
	va_end(args);
}

jmp_buf ex;
r6502 cpu(&memory, &ex, status);
bool halted = false;
const char *filename;

void reset() {
	bool sd = hardware_reset();

	io.reset();
	if (sd)
		io.tape.start(PROGRAMS);
	else
		io.status("No SD Card");

	halted = (setjmp(ex) != 0);
}

void setup() {
	Serial.begin(115200);
	Serial.println("hello world");
	hardware_init(cpu);
		
	for (unsigned i = 0; i < RAM_SIZE; i += 1024)
		memory.put(pages[i / 1024], i);

	memory.put(sram, SPIRAM_BASE, SPIRAM_EXTENT);
	memory.put(io, 0xd000);
	memory.put(b, 0xe000);
	memory.put(m, 0xff00);

	reset();
}

void loop() {
	if (ps2.available()) {
		unsigned key = ps2.read();
		if (!ps2.isbreak())
			io.down(key);
		else
			switch (key) {
			case PS2_F1:
				reset();
				break;
			case PS2_F2:
				filename = io.tape.advance();
				io.status(filename);
				break;
			case PS2_F3:
				filename = io.tape.rewind();
				io.status(filename);
				break;
			case PS2_F4:
				io.load();
				break;
			case PS2_F6:
				io.status(checkpoint(io.tape, PROGRAMS));
				break;
			case PS2_F7:
				if (filename)
					restore(io.tape, PROGRAMS, filename);
				break;
			default:
				io.up(key);
				break;
			}
	} else if (!halted)
		cpu.run(CPU_INSTRUCTIONS);
}
