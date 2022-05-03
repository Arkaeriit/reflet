# Flags
FLAGS := -Wall -g -Werror -Wextra -Wpedantic

# Files lists
C_SRC := config.c debug.c loader.c main.c reflet.c
C_OBJS := $(C_SRC:%.c=%.o)
TARGET := reflet-sim

# Install targets
TARGET_DIR_BIN := /usr/local/bin
TARGET_BIN := $(TARGET_DIR_BIN)/$(TARGET)

# Commands
CC := gcc
CP := cp -f
RM := rm -rf

all : $(TARGET).bin

%.o : %.c
	$(CC) -c $< $(FLAGS) -o $@

$(TARGET).bin : $(C_OBJS)
	$(CC) $(C_OBJS) $(FLAGS) -o $@

install :
	mkdir -p $(TARGET_DIR_BIN)
	$(CP) $(TARGET).bin $(TARGET_BIN)

uninstall :
	$(RM) $(TARGET_BIN)

clean : 
	$(RM) *.bin
	$(RM) *.o

