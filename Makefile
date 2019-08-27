
# toolchain
TOOLCHAIN    = arm-none-eabi-
CC           = $(TOOLCHAIN)gcc
CP           = $(TOOLCHAIN)objcopy
AS           = $(TOOLCHAIN)gcc -x assembler-with-cpp
HEX          = $(CP) -O ihex
BIN          = $(CP) -O binary -S

# define mcu, specify the target processor
MCU          = cortex-m0plus

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJECT_NAME=nv32-gcc-example


# user specific
ASM_SRC   = Synth_m0.s


OBJECTS  = $(ASM_SRC:.s=.o)

# Define optimisation level here
OPT = -Os

MC_FLAGS = -mcpu=$(MCU)

AS_FLAGS = $(MC_FLAGS) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst)
#
# makefile rules
#
all: $(OBJECTS)


%.o: %.s
	$(AS) -c $(AS_FLAGS) $< -o $@

clean:
	-rm -rf $(OBJECTS)
	-rm -rf $(ASM_SRC:.s=.lst)

