ENDIAN         := EL

HOST_CC        := cc
HOST_CFLAGS    += --std=gnu99 -O2 -Wall
HOST_ENDIAN    := $(shell lscpu | grep -iq 'little endian' && echo EL || echo EB)
ifneq ($(HOST_ENDIAN), $(ENDIAN))
HOST_CFLAGS    += -DCONFIG_REVERSE_ENDIAN
endif

.PHONY: clean

all: fsformat

%: %.c
	@echo [CC] $@
	@$(HOST_CC) $(HOST_CFLAGS) -o $@ $^

clean:
	rm fsformat
