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
- F1 (^N): reset
- F2 (^O): advance tape
- F3 (^P): rewind tape
- F5 (^R): load program from tape (by simulating typing it)
- F7 (^T): checkpoint machine
- F8 (^U): restore from current checkpoint on tape
- F10(^W): watch CPU execute instructions
- Underscore is rubout
