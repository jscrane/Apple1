#include <Arduino.h>
#include <timed.h>

#include "disp.h"

static disp *i;

const int TICK_FREQ = 2;

bool disp::start() {

	i = this;
	timer_create(TICK_FREQ, disp::on_tick);
	return true;
}

void IRAM_ATTR disp::on_tick() {

	static int tick = 0;
	tick = ++tick % 3;
	i->cursor(tick < 2);
}
