LIB_SRC = $(wildcard lib/*.c)
LIB_SRC_OBJ = $(patsubst %.c,%.o,${LIB_SRC})

LIB_ASM = $(wildcard lib/*.S)
LIB_ASM_OBJ = $(patsubst %.S,%.o,${LIB_ASM})

LIB_OBJ = ${LIB_ASM_OBJ} ${LIB_SRC_OBJ}
SHARE_LIB = $(addprefix ../,${SHARE_LIB_OBJ})

APP_SRC = $(wildcard app/*.c)
APP_EXE = $(patsubst %.c,%.b,${APP_SRC})