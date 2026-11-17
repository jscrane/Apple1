Apple-1 Emulator
================
- Emulates an Apple-1 with up to 52k of RAM
- See [napple1](https://github.com/nobuh/napple1),
[krusader](http://school.anhb.uwa.edu.au/personalpages/kwessen/apple1/Krusader.htm)
and more [software](http://www.willegal.net/appleii/apple1-software.htm)

Requirements
------------
- [6502 Emulation](https://github.com/jscrane/r65emu) library
- [uC-Makefile](https://github.com/jscrane/uC-Makefile)

Keyboard
--------
- F1: reset
- F2: advance tape
- F3: rewind tape
- F4: load program from tape (by simulating typing it)
- Underscore is rubout
- If using console keyboard (e.g., with `minicom`), and see "double-line spacing",
  switch off "Add linefeed" (minicom: `^A Z A`)
