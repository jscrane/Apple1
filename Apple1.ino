#include <stdarg.h>
#include <SPI.h>

#include <r65emu.h>
#include <r6502.h>
#include <pia.h>
#include <sd_filer.h>

#include "io.h"
#include "disp.h"
#include "screen_disp.h"
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

#elif defined(HW_SERIAL_KBD)
hw_serial_kbd kbd(Serial);

#else
#error "No keyboard defined!"
#endif

screen_disp dsp;
io io(files, kbd, dsp);

r6502 cpu(memory);

void reset() {
	bool ok = hardware_reset();

	io.reset();
	if (!ok) {
		dsp.status("Reset failed");
		return;
	}
	io.start();
#if defined(KRUSADER)
	dsp.status("Krusader: F000R / Basic: E000R");
#else
	dsp.status("Basic: E000R");
#endif
}

void function_key(uint8_t fn) {
	static const char *filename;

	switch(fn) {
	case 1:
		reset();
		break;
	case 2:
		filename = io.files.advance();
		dsp.status(filename);
		break;
	case 3:
		filename = io.files.rewind();
		dsp.status(filename);
		break;
	case 4:
		io.load();
		break;
	case 6:
		dsp.status(io.files.checkpoint());
		break;
	case 7:
		if (filename)
			io.files.restore(filename);
		break;
	case 10:
		hardware_debug_cpu();
		break;
	}
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
		memory.put(pages[i], i * ram<>::page_size);

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

	kbd.register_fnkey_handler(function_key);

	reset();
}

void loop() {

	hardware_run();
}
