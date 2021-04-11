# Flags
FLAGS := -Wall -Werror -g -fPIC
LUA = -llua -lm -ldl   
REFLETPASM = -lreflet-pasm

# Files lists
C_SRC := libreflet-masm.c macro_assembler.c
C_OBJS := $(C_SRC:%.c=%.o)
LUA_SRC := argFlags.lua lib.lua link.lua macro_assembler.lua macro.lua readingCode.lua
LUA_OBJS := $(LUA_SRC:%.lua=%.luac)
TARGET := reflet-masm

# Install targets
TARGET_DIR_BIN := /usr/local/bin
TARGET_BIN := $(TARGET_DIR_BIN)/$(TARGET)
LIBPATH := /usr/lib64
INCLUDEPATH := /usr/include
TARGET_LIB := $(INCLUDEPATH)/lib$(TARGET).so

# Commands
CC := gcc
CP := cp -f
RM := rm -rf

all : $(TARGET).bin lib$(TARGET).so

%.o : %.c
	$(CC) -c $< $(FLAGS) -o $@

%.lua : %.c
	luac -o $@ $<

$(TARGET).bin : macro_assembler.o
	$(CC) macro_assembler.o $(FLAGS) $(LUA) $(REFLETPASM) -o $@

lib$(TARGET).so : libreflet-masm.o
	$(CC) libreflet-masm.o $(FLAGS) -shared -o $@

install :
	mkdir -p $(TARGET_DIR_BIN)
	$(CP) $(TARGET).bin $(TARGET_BIN)
	mkdir -p $(INCLUDEPATH)
	$(CP) libreflet-masm.h $(INCLUDEPATH)/reflet-masm.h
	mkdir -p $(LIBPATH)
	$(CP) lib$(TARGET).so $(LIBPATH)

uninstall :
	$(RM) $(TARGET_BIN)
	$(RM) $(INCLUDEPATH)/reflet-pasm.h
	$(RM) $(LIBPATH)/lib$(TARGET).so

clean : 
	$(RM) *.bin
	$(RM) *.o
	$(RM) *.so

