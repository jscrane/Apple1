PROCESSOR_FAMILY := lm4f
BOARD := lplm4f120h5qr
# watch the CPU execute
#CPPFLAGS := -DCPU_DEBUG
CPPFLAGS := -DHARDWARE_H=\"hw/lm4f-utft-sd.h\"

include energia10.mk
