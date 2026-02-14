#include <stdarg.h>
#include <SPI.h>

#include <r65emu.h>
#include <r6502.h>
#include <pia.h>
#include <debugging.h>

#include "io.h"
#include "disp.h"
#include "screen_disp.h"
#include "terminal_disp.h"
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

ram<> pages[RAM_PAGES];
//socket_filer files("apple1");
#if defined(USE_SD)
sd_filer files(PROGRAMS);
#else
flash_filer files(PROGRAMS);
#endif

#if defined(PS2_SERIAL_KBD)
ps2_serial_kbd kbd;
#else
hw_serial_kbd kbd(Serial);
#endif

#if defined(SCREEN_SERIAL_DSP)
screen_disp dsp;
#else
terminal_disp dsp(Serial);
#endif
io io(files, kbd, dsp);

Memory memory;
r6502 cpu(memory);
Arduino machine(cpu);

static void reset(bool ok) {

	io.reset();
	if (!ok) {
		DBG_EMU("Reset failed");
		dsp.status("Reset failed");
		return;
	}
	if (!io.start()) {
		DBG_EMU("IO Start failed");
		dsp.status("IO Start failed");
		return;
	}
#if defined(KRUSADER)
	dsp.status("Krusader: F000R / Basic: E000R");
#else
	dsp.status("Basic: E000R");
#endif
}

static const char *open(const char *filename) {
	if (filename) {
		dsp.status(filename);
		return filename;
	}
	dsp.status("No file");
	return 0;
}

static void function_key(uint8_t fn) {
	static const char *filename;

	switch(fn) {
	case 1:
		machine.reset();
		break;
	case 2:
		filename = open(io.files.advance());
		break;
	case 3:
		filename = open(io.files.rewind());
		break;
	case 5:
		io.load();
		break;
	case 7:
		dsp.status(io.files.checkpoint());
		break;
	case 8:
		if (filename)
			io.files.restore(filename);
		break;
	case 10:
		machine.debug_cpu();
		break;
	}
}

void setup() {

	machine.begin();

	DBG_INI("RAM:    %dkB at 0x0000", RAM_PAGES);

	for (unsigned i = 0; i < RAM_PAGES; i++)
		memory.put(pages[i], i * ram<>::page_size);

#if defined(USE_SPIRAM)
	DBG_INI("SpiRAM: %dkB at 0x%04x", SPIRAM_EXTENT * Memory::page_size / 1024, SPIRAM_BASE);

	memory.put(sram, SPIRAM_BASE, SPIRAM_EXTENT);
#endif

	memory.put(io, 0xd000);

#if defined(KRUSADER)
	memory.put(k, 0xe000);
#else
	memory.put(b, 0xe000);
	memory.put(m, 0xff00);
#endif

	kbd.register_fnkey_handler(function_key);

	machine.register_reset_handler(reset);
	machine.reset();
}

void loop() {

	machine.run();
}
