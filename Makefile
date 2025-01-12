t ?= esp32

TERMINAL_SPEED := 115200
#CPPFLAGS = -Wall -DDEBUGGING -DCPU_DEBUG=false -DTERMINAL_SPEED=$(TERMINAL_SPEED) -DUSE_OWN_KBD
CPPFLAGS = -Wall -DTERMINAL_SPEED=$(TERMINAL_SPEED) -DUSE_OWN_KBD
LIBRARIES = PS2KeyAdvanced PS2KeyMap SimpleTimer

ifeq ($t, esp8266)
BOARD := d1_mini
BAUD := 921600
EESZ := 4M1M
F_CPU := 80
CPPFLAGS += -DUSER_SETUP_LOADED -DILI9341_DRIVER -DTFT_CS=PIN_D8 -DTFT_DC=PIN_D1 \
	-DTFT_RST=-1 -DSPI_FREQUENCY=40000000 -DLOAD_GLCD \
	-DHARDWARE_H=\"hw/esp8bit.h\"
LIBRARIES += TFT_eSPI SpiRAM LittleFS
endif

ifeq ($t, tivac)
BOARD := EK-LM4F120XL
CPPFLAGS += -DHARDWARE_H=\"hw/stellarpad-example.h\" -DPS2_SERIAL_KBD=\"UK\"
LIBRARIES += UTFT SD SpiRAM
endif

ifeq ($t, esp32)
UPLOADSPEED := 921600
LIBRARIES += FS SPIFFS Network

ifeq ($b, lilygo)
BOARD := ttgo-t7-v14-mini32
SERIAL_PORT := /dev/ttyACM0
CPPFLAGS += -DHARDWARE_H=\"hw/ttgo-t7-v14-mini32.h\"
LIBRARIES += ESP32Lib

else
BOARD := lolin32
CPPFLAGS += -DUSER_SETUP_LOADED -DILI9341_DRIVER -DTFT_CS=5 -DTFT_DC=2 \
	-DTFT_RST=-1 -DSPI_FREQUENCY=40000000 -DLOAD_GLCD \
	-DHARDWARE_H=\"hw/esp32-example.h\" -DUSE_SERIAL
LIBRARIES += TFT_eSPI
endif
endif

ifeq ($t, rp2040)
BOARD := adafruit_feather_dvi
FLASH := 8388608_2097152
CPPFLAGS += -DHARDWARE_H=\"hw/adafruit_feather_dvi.h\" -DUSE_SERIAL
#CPPFLAGS += -DHARDWARE_H=\"hw/adafruit_feather_dvi.h\" -DPS2_SERIAL_KBD=\"UK\"
CPPFLAGS += -DDVI_BIT_DEPTH=1 -DDVI_RESOLUTION=DVI_RES_640x240p60
LIBRARIES += LittleFS PicoDVI Adafruit_GFX Adafruit_BusIO Wire
endif

include $t.mk
