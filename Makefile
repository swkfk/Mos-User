include include.mk

LIB_SRC = $(wildcard lib/*.c)
SHARE_LIB_OBJ = $(patsubst %.c,%.o,${LIB_SRC})
TARGET = target/user

export CC CFLAGS LD LDFLAGS SHARE_LIB_OBJ TARGET

.PHONY: mkdir user fs tools clean fs-image

all: mkdir fs-image

mkdir:
	@mkdir -p ${TARGET}

fs:
	@$(MAKE) --directory=$@

tools:
	@$(MAKE) --directory=$@

user:
	@$(MAKE) --directory=$@

fs-image: tools user fs
	@$(MAKE) --directory=fs image fs-files="$(addprefix ../, $(fs-files))"
	@echo [CP] icode.b
	@cp user/app/icode.b ${TARGET}/
	@echo [CP] serv.b
	@cp fs/serv.b ${TARGET}/

clean:
	-rm **/*.o **/*.b
	-$(MAKE) --directory=tools clean
	-$(MAKE) --directory=fs clean
	-rm -r ${TARGET}
