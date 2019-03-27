#include <stdarg.h>
#include <SPI.h>

#include <r65emu.h>
#include <r6502.h>

#include "pia.h"
#include "io.h"
#include "config.h"

#if defined(KRUSADER)
#include "roms/krusader6502.h"
prom b(krusader6502, sizeof(krusader6502));
#else
#include "roms/basic.h"
#include "roms/monitor.h"
prom b(basic, sizeof(basic));
prom m(monitor, sizeof(monitor));
#endif

ram pages[RAM_SIZE / 1024];
flash_filer files(PROGRAMS);
io io(files);

r6502 cpu(memory);
const char *filename;

void reset() {
	bool sd = hardware_reset();

	io.reset();
	if (sd)
		io.files.start();
	else
		io.status("No SD Card");
}

void setup() {
	Serial.begin(115200);
	hardware_init(cpu);
		
	for (unsigned i = 0; i < RAM_SIZE; i += 1024)
		memory.put(pages[i / 1024], i);

#if defined(USE_SPIRAM)
	memory.put(sram, SPIRAM_BASE, SPIRAM_EXTENT);
#endif
	memory.put(io, 0xd000);
#if defined(KRUSADER)
	memory.put(b, 0xe000);
#else
	memory.put(b, 0xe000);
	memory.put(m, 0xff00);
#endif

	reset();
}

void loop() {
	if (ps2.available()) {
		unsigned scan = ps2.read2();
		byte key = scan & 0xff;
		if (is_down(scan))
			io.down(key);
		else
			switch (key) {
			case PS2_F1:
				reset();
				break;
			case PS2_F2:
				filename = io.files.advance();
				io.status(filename);
				break;
			case PS2_F3:
				filename = io.files.rewind();
				io.status(filename);
				break;
			case PS2_F4:
				io.load();
				break;
			case PS2_F6:
				io.status(io.files.checkpoint());
				break;
			case PS2_F7:
				if (filename)
					io.files.restore(filename);
				break;
			default:
				io.up(key);
				break;
			}
	} else if (!cpu.halted())
		cpu.run(CPU_INSTRUCTIONS);
}
