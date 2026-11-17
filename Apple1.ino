#include <stdarg.h>
#include <SPI.h>

#include <r65emu.h>
#include <r6502.h>
#include <pia.h>
#include <debugging.h>
#include <flash_filer.h>
#include <display.h>

#include "io.h"
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

#if defined(USE_SPIRAM)
#include <spiram.h>
#endif

ram<> pages[RAM_PAGES];
flash_device tape("Tape");

#if defined(USE_HOST_DISPLAY)
terminal_disp dsp(Serial);
#else
Display display;
screen_disp dsp(display);
#endif

Memory memory;
r6502 cpu(memory);
Arduino machine(cpu);
PIA pia;
io io(pia, tape, machine.console_keyboard(), dsp);

static void reset(bool sd) {

	io.reset();
	if (!sd) {
		DBG_EMU("No SD Card");
		dsp.status("No SD Card");
	} else if (!tape.start(PROGRAMS)) {
		DBG_EMU("Failed to open " PROGRAMS);
		dsp.status("Failed to open " PROGRAMS);
	} else {
#if defined(KRUSADER)
		dsp.status("Krusader: F000R / Basic: E000R");
#else
		dsp.status("Basic: E000R");
#endif
	}
}

static void opened(const char *filename) {
	dsp.status(filename? filename: "No file");
}

static void function_key(uint16_t fn) {
	switch(fn) {
	case F1:
		machine.reset();
		break;
	case F2:
		opened(tape.advance());
		break;
	case F3:
		opened(tape.rewind());
		break;
	case F4:
		io.load();
		break;
	case F10:
		dsp.statusf("%dHz", machine.current_speed());
		break;
	}
}

void setup() {

	machine.begin();

	DBG_INI("RAM:    %dkB at 0x0000", RAM_PAGES);
	for (unsigned i = 0; i < RAM_PAGES; i++)
		memory.put(pages[i], i * ram<>::page_size);

#if defined(USE_SPIRAM)
	DBG_INI("SpiRAM: %dkB at 0x%04x", SPIRAM_EXTENT / 1024, SPIRAM_BASE);
	memory.put(sram, SPIRAM_BASE, SPIRAM_EXTENT);
#endif

	memory.put(pia, 0xd000);

#if defined(KRUSADER)
	memory.put(k, 0xe000);
#else
	memory.put(b, 0xe000);
	memory.put(m, 0xff00);
#endif

	io.register_fnkey_handler(function_key);

	machine.register_pollable(io);
	machine.register_reset_handler(reset);
	machine.reset();
}

void loop() {

	machine.run(1023000);
}
