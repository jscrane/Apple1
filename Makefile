PROCESSOR_FAMILY := lm4f
BOARD := lplm4f120h5qr
CPPFLAGS := -DDEBUGGING -DHARDWARE_H=\"hw/lm4f-utft-sd.h\"
LIBRARIES := UTFT SD

include energia10.mk
