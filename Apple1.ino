#include <stdarg.h>
#include <SPI.h>

#include <r65emu.h>
#include <r6502.h>
#include <pia.h>
#include <sd_filer.h>

#include "io.h"
#include "config.h"

#if defined(KRUSADER)
#include "roms/krusader6502.h"
prom k(krusader6502, sizeof(krusader6502));
#else
#include "roms/basic.h"
#include "roms/monitor.h"
prom b(basic, sizeof(basic));
prom m(monitor, sizeof(monitor));
#endif

ram pages[RAM_PAGES];
//socket_filer files("apple1");
#if defined(USE_SD)
sd_filer files(PROGRAMS);
#else
flash_filer files(PROGRAMS);
#endif
io io(files);

r6502 cpu(memory);
const char *filename;

void reset() {
	bool ok = hardware_reset();

	io.reset();
	if (!ok) {
		io.status("Reset failed");
		return;
	}
	io.start();
#if defined(KRUSADER)
	io.status("Krusader: F000R / Basic: E000R");
#else
	io.status("Basic: E000R");
#endif
}

void setup() {
#if defined(DEBUGGING) || defined(CPU_DEBUG)
	Serial.begin(TERMINAL_SPEED);
	Serial.println();
	Serial.print("RAM:    ");
	Serial.print(RAM_PAGES);
	Serial.print("kB at 0x0000");
	Serial.println();
#if defined(USE_SPIRAM)
	Serial.print("SpiRAM: ");
	Serial.print(SPIRAM_EXTENT * Memory::page_size / 1024);
	Serial.print("kB at 0x");
	Serial.print(SPIRAM_BASE, 16);
	Serial.println();
#endif
#endif
	hardware_init(cpu);
		
	for (unsigned i = 0; i < RAM_PAGES; i++)
		memory.put(pages[i], i * ram::page_size);

#if defined(USE_SPIRAM)
	memory.put(sram, SPIRAM_BASE, SPIRAM_EXTENT);
#endif

	memory.put(io, 0xd000);

#if defined(KRUSADER)
	memory.put(k, 0xe000);
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
