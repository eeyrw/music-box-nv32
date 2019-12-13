
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
PROJECT_NAME=music-box-nv32

# specify define
DDEFS       =

# define root dir
ROOT_DIR     = .

# define include dir
INCLUDE_DIRS =

# define nv32 lib dir
NV32_LIB_DIR      = $(ROOT_DIR)/nv32lib

# define user dir
USER_DIR     = $(ROOT_DIR)

# link file
LINK_SCRIPT  = $(ROOT_DIR)/nv32f100.ld

# user specific
SRC       =
ASM_SRC   =


SRC      += $(USER_DIR)/main.c
SRC      += $(USER_DIR)/gcc-stdio-retarget.c
SRC      += $(USER_DIR)/KeyScan.c
SRC      += $(USER_DIR)/DownloadScoreData.c

SRC      += $(ROOT_DIR)/WaveTableSynthesizer/SynthCore.c
SRC      += $(ROOT_DIR)/WaveTableSynthesizer/EnvelopTable.c
SRC      += $(ROOT_DIR)/WaveTableSynthesizer/AlgorithmTest.c
SRC      += $(ROOT_DIR)/WaveTableSynthesizer/WaveTable_Celesta_C5.c
SRC      += $(ROOT_DIR)/WaveTableSynthesizer/WaveTable_Celesta_C6.c
SRC      += $(ROOT_DIR)/WaveTableSynthesizer/Player.c

SRC      += $(ROOT_DIR)/Ring-Buffer/ringbuffer.c

ASM_SRC  += $(ROOT_DIR)/WaveTableSynthesizer/PeriodTimer.s
ASM_SRC  += $(ROOT_DIR)/WaveTableSynthesizer/SynthCoreAsm.s
# ASM_SRC  += $(ROOT_DIR)/ScoreList.s


# user include
INCLUDE_DIRS  = $(USER_DIR)
INCLUDE_DIRS  += $(ROOT_DIR)/WaveTableSynthesizer
INCLUDE_DIRS  += $(ROOT_DIR)/Ring-Buffer

# include sub makefiles
include makefile_std_lib.mk   # NV32 Library

INC_DIR  = $(patsubst %, -I%, $(INCLUDE_DIRS))

# run from Flash
DEFS	 = $(DDEFS) -DRUN_FROM_FLASH=1

OBJECTS  = $(ASM_SRC:.s=.o) $(SRC:.c=.o) scoreList.o

# Define optimisation level here
OPT = -Os

MC_FLAGS = -mcpu=$(MCU)

AS_FLAGS = $(MC_FLAGS) -g -gdwarf-2 -mthumb  -Wa,-amhls=$(<:.s=.lst)
CP_FLAGS = $(MC_FLAGS) $(OPT) -fdata-sections -ffunction-sections -g -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -fverbose-asm -Wa,-ahlms=$(<:.c=.lst) $(DEFS)
LD_FLAGS = $(MC_FLAGS) -g -specs=nano.specs -specs=nosys.specs -gdwarf-2 -mthumb -nostartfiles -Xlinker -T$(LINK_SCRIPT) -Wl,-Map=$(PROJECT_NAME).map,--cref,--no-warn-mismatch,--gc-sections

#
# makefile rules
#
all: $(OBJECTS) $(PROJECT_NAME).elf  $(PROJECT_NAME).hex $(PROJECT_NAME).bin
	$(TOOLCHAIN)size $(PROJECT_NAME).elf

%.o: %.c Makefile makefile_std_lib.mk
	@echo [CC] $(notdir $<)
	@$(CC) -c $(CP_FLAGS) -I . $(INC_DIR) $< -o $@

%.o: %.s
	@echo [AS] $(notdir $<)
	@$(AS) -c $(AS_FLAGS) $< -o $@
	
scoreList.o: scoreList.raw
	@echo [RAW] scoreList.raw
	@$(CP) -I binary -O elf32-littlearm scoreList.raw scoreList.o

%.elf: $(OBJECTS)  $(LINK_SCRIPT)
	@echo [LD] $(PROJECT_NAME).elf
	@$(CC) $(OBJECTS) $(LD_FLAGS) -o $@

%.hex: %.elf
	@echo [HEX] $(PROJECT_NAME).hex
	@$(HEX) $< $@

%.bin: %.elf
	@echo [BIN] $(PROJECT_NAME).bin
	@$(BIN)  $< $@

flash: $(PROJECT_NAME).bin
	st-flash write $(PROJECT_NAME).bin 0x8000000

erase:
	st-flash erase

clean:
	@echo [RM] OBJ
	@-rm -rf $(OBJECTS)
	@echo [RM] BIN "(ELF,HEX,BIN)"
	@-rm -rf $(PROJECT_NAME).elf
	@-rm -rf $(PROJECT_NAME).hex
	@-rm -rf $(PROJECT_NAME).bin
	@echo [RM] LST
	@-rm -rf $(SRC:.c=.lst)
	@-rm -rf $(ASM_SRC:.s=.lst)
	@echo [RM] MAP
	@-rm -rf $(PROJECT_NAME).map

