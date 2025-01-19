include include.mk

INCLUDES = -I../include -Iinclude
USER_LDS = user.lds

all: ${APP_EXE}

%.b: %.o $(LIB_OBJ) $(SHARE_LIB)
	@echo [$(HINT)-LD] $@
	@$(LD) -o $@ $(LDFLAGS) -T $(USER_LDS) $^

%.o: %.c
	@echo [$(HINT)-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	@echo [$(HINT)-CC] $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf **/*.o **/*.b
