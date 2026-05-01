CC = gcc
LD = ld

SRC_DIR = origin
OUT_DIR = ../kernel/luabridge

CFLAGS = -O2 -ffreestanding -mno-red-zone -mcmodel=kernel \
         -fno-pic -fno-pie -fno-stack-protector -U_FORTIFY_SOURCE \
         -DLUA_ANSI -D__I386__ -D__x86_64__ -DLUA_USE_C89 \
         -DLUA_COMPAT_5_3=0 -nostdlib -I$(SRC_DIR)

# Lua core only
CORE_SRC = lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c \
           lmem.c lobject.c lopcodes.c lstate.c lstring.c ltable.c \
           ltm.c lundump.c lvm.c lzio.c

# Minimal libraries: just base lib
LIB_SRC  = lauxlib.c lbaselib.c

OBJS = $(addprefix $(SRC_DIR)/, $(CORE_SRC:.c=.o) $(LIB_SRC:.c=.o))

all: clean setup $(OUT_DIR)/lua.o

setup:
	cp origin/luaconf.h backupconf.h
	cp luaconf.h origin/luaconf.h
	@mkdir -p $(OUT_DIR)

$(OUT_DIR)/lua.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(OUT_DIR)/lua.o
	cp backupconf.h origin/luaconf.h