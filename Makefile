PROCESSOR_FAMILY := lm4f
BOARD := lplm4f120h5qr
# watch the CPU execute
#LOCAL_CPPFLAGS := -DCPU_DEBUG
CXXFLAGS := -fno-operator-names

include energia.mk
