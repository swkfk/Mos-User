user_dir    := ../user
tools_dir   := ../tools
INCLUDES    := -I../include -I$(user_dir)/include

LIB_SRC_OBJ = $(patsubst %.c,%.o,$(wildcard $(user_dir)/lib/*.c))
LIB_ASM_OBJ = $(patsubst %.S,%.o,$(wildcard $(user_dir)/lib/*.S))
LIB_SHR_OBJ = $(addprefix ../,${SHARE_LIB_OBJ})
LIB_OBJ = ${LIB_SRC_OBJ} ${LIB_ASM_OBJ} ${LIB_SHR_OBJ}
APP_EXE := $(patsubst %.c,%.b,$(wildcard $(user_dir)/app/*.c))

FSLIB       := fs.o ide.o
FSIMGFILES  := rootfs/motd rootfs/newmotd $(APP_EXE)

%.b: %.o $(LIB_OBJ) $(FSLIB)
	$(LD) -o $@ $(LDFLAGS) -T $(user_dir)/user.lds $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: $(user_dir)/lib.h

.PHONY: all clean image

all: serv.b

clean:
	rm ../${TARGET}/*.img

image: $(tools_dir)/fsformat
	dd if=/dev/zero of=../${TARGET}/fs.img bs=4096 count=1024 2>/dev/null
	dd if=/dev/zero of=../${TARGET}/empty.img bs=4096 count=1024 2>/dev/null
	# using awk to remove paths with identical basename from FSIMGFILES
	$(tools_dir)/fsformat ../${TARGET}/fs.img \
		$$(printf '%s\n' $(FSIMGFILES) | awk -F/ '{ ns[$$NF]=$$0 } END { for (n in ns) { print ns[n] } }')
