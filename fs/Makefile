user_dir    := ../user
tools_dir   := ../tools
INCLUDES    := -I../include -I$(user_dir)/include

LIB_SRC_OBJ = $(patsubst %.c,%.o,$(wildcard $(user_dir)/lib/*.c))
LIB_ASM_OBJ = $(patsubst %.S,%.o,$(wildcard $(user_dir)/lib/*.S))
LIB_SHR_OBJ = $(addprefix ../,${SHARE_LIB_OBJ})
LIB_OBJ = ${LIB_SRC_OBJ} ${LIB_ASM_OBJ} ${LIB_SHR_OBJ}
APP_EXE := $(patsubst %.c,%.b,$(wildcard $(user_dir)/app/*.c))

RTFILES = $(wildcard rootfs/*)

FSLIB       := fs.o ide.o
FSIMGFILES  := $(RTFILES) $(APP_EXE)

%.b: %.o $(LIB_OBJ) $(FSLIB)
	@echo [mips-LD] $@
	@$(LD) -o $@ $(LDFLAGS) -T $(user_dir)/user.lds $^

%.o: %.c
	@echo [mips-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	@echo [mips-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: $(user_dir)/lib.h

.PHONY: all clean image

all: serv.b

clean:
	rm ../${TARGET}/*.img

image: $(tools_dir)/fsformat
	@echo [FS-dd] fs.img
	@dd if=/dev/zero of=../${TARGET}/fs.img bs=4096 count=1024 2>/dev/null
	@echo [FS-dd] empty.img
	@dd if=/dev/zero of=../${TARGET}/empty.img bs=4096 count=1024 2>/dev/null
	@$(tools_dir)/fsformat ../${TARGET}/fs.img \
		$$(printf '%s\n' $(FSIMGFILES) | awk -F/ '{ ns[$$NF]=$$0 } END { for (n in ns) { print ns[n] } }')
