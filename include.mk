ENDIAN         := EL

QEMU           := qemu-system-mipsel
CROSS_COMPILE  := mips-linux-gnu-
CC             := $(CROSS_COMPILE)gcc
CFLAGS         += --std=gnu99 -$(ENDIAN) -G 0 -mno-abicalls -fno-pic \
                  -ffreestanding -fno-stack-protector -fno-builtin \
                  -Wa,-xgot -Wall -mxgot -mno-fix-r4000 -march=24kc
LD             := $(CROSS_COMPILE)ld
LDFLAGS        += -$(ENDIAN) -G 0 -static -n -nostdlib --fatal-warnings

ifneq ($(MOS_RELEASE),)
	CFLAGS   := $(CFLAGS) -O2
	LDFLAGS  := $(LDFLAGS) -O --gc-sections
else
	CFLAGS   := $(CFLAGS) -O0 -g -ggdb -DMOS_DEBUG
    LDFLAGS  := $(LDFLAGS)
endif
