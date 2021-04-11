# Flags
FLAGS := -Wall -Werror -g -fPIC

# Files lists
C_SRC := pico_assembler.c pico_assembler_main.c
C_OBJS := $(C_SRC:%.c=%.o)
TARGET := reflet-pasm

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

$(TARGET).bin : $(C_OBJS)
	$(CC) $(C_OBJS) $(FLAGS) -o $@

lib$(TARGET).so : $(C_OBJS)
	$(CC) $(C_OBJS) $(FLAGS) -shared -o $@

install :
	mkdir -p $(TARGET_DIR_BIN)
	$(CP) $(TARGET).bin $(TARGET_BIN)
	mkdir -p $(INCLUDEPATH)
	$(CP) pico_assembler.h $(INCLUDEPATH)/reflet-pasm.h
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

