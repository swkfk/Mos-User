include include.mk

INCLUDES = -I../include -Iinclude
USER_LDS = user.lds

all: ${APP_EXE}

%.b: %.o $(LIB_OBJ) $(SHARE_LIB)
	@echo [mips-LD] $@
	@$(LD) -o $@ $(LDFLAGS) -T $(USER_LDS) $^

%.o: %.c
	@echo [mips-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	@echo [mips-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf **/*.o **/*.b
