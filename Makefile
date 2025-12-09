TARGET = timer

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# OPEN OCD
# Access the environment variable MY_FOLDER_PATH
# NOTE: Ensure IFX_BARE_METAL_COURSE environment variable is set
OPENOCD_DIR := $(IFX_BARE_METAL_COURSE)/openocd
OCD = $(OPENOCD_DIR)
OCD_Scripts = $(OCD)/scripts
OCD_Interface = $(OCD)/scripts/interface/kitprog3.cfg
OCD_Target = $(OCD)/scripts/target/psoc4.cfg

# Flags
CFLAGS = -mcpu=cortex-m0plus -mthumb -g -Wall -O0
LDFLAGS = -mcpu=cortex-m0plus -mthumb -T psoclinker.ld -Wl,-Map=$(TARGET).map -nostartfiles

# Files
# SRCS updated to match existing file startup_psoc4.c
SRCS = main.c psoc_startup.c timer.c
OBJS = $(SRCS:.c=.o)
HEX = $(TARGET).hex

# Rules
all: $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).map

# OpenOCD command to program a board
program: $(HEX)
	$(OCD)/bin/openocd.exe -f $(OCD_Interface) -f $(OCD_Target) -c 'program $(HEX) verify reset exit'

# OpenOCD command to ERASE a board
erase:
	$(OCD)/bin/openocd.exe -f $(OCD_Interface) -f $(OCD_Target) -c 'kitprog3 power_config on 3300; init; reset init; psoc4 mass_erase 0; shutdown; kitprog3 power_config off; exit'
