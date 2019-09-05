
# STD Defines
DDEFS += -DNV32

# source director
NV32_DRIVER_LIB     = $(NV32_LIB_DIR)/drivers
NV32_CPU_LIB    = $(NV32_LIB_DIR)/cpu
NV32_COMMON_DIR    = $(NV32_LIB_DIR)/common
NV32_PLATFORM_DIR  = $(NV32_LIB_DIR)/platforms

# startup
ASM_SRC  += $(NV32_CPU_LIB)/startup_NV32_gcc.s


# use libraries, please add or remove when you use or remove it.
# SRC  += $(NV32_COMMON_DIR)/alloc.c
SRC  += $(NV32_COMMON_DIR)/assert.c
# SRC  += $(NV32_COMMON_DIR)/io.c
# SRC  += $(NV32_COMMON_DIR)/memtest.c
# SRC  += $(NV32_COMMON_DIR)/queue.c
# SRC  += $(NV32_COMMON_DIR)/memtest.c
# SRC  += $(NV32_COMMON_DIR)/stdlib.c

SRC  += $(NV32_CPU_LIB)/arm_cm0.c
SRC  += $(NV32_CPU_LIB)/start.c
SRC  += $(NV32_CPU_LIB)/sysinit.c
SRC  += $(NV32_CPU_LIB)/vectors.c

# SRC  += $(NV32_DRIVER_LIB)/acmp/acmp.c
SRC  += $(NV32_DRIVER_LIB)/adc/adc.c
# SRC  += $(NV32_DRIVER_LIB)/crc/crc.c
SRC  += $(NV32_DRIVER_LIB)/gpio/gpio.c
SRC  += $(NV32_DRIVER_LIB)/ics/ics.c
# SRC  += $(NV32_DRIVER_LIB)/iic/i2c.c
# SRC  += $(NV32_DRIVER_LIB)/kbi/kbi.c
SRC  += $(NV32_DRIVER_LIB)/nvm/flash.c
SRC  += $(NV32_DRIVER_LIB)/eeprom/eeprom.c
SRC  += $(NV32_DRIVER_LIB)/pit/pit.c
SRC  += $(NV32_DRIVER_LIB)/PMC/pmc.c
SRC  += $(NV32_DRIVER_LIB)/rtc/rtc.c
SRC  += $(NV32_DRIVER_LIB)/sim/sim.c
# SRC  += $(NV32_DRIVER_LIB)/spi/spi.c
SRC  += $(NV32_DRIVER_LIB)/uart/uart.c
SRC  += $(NV32_DRIVER_LIB)/etm/etm.c
SRC  += $(NV32_DRIVER_LIB)/delay/delay.c
SRC  += $(NV32_DRIVER_LIB)/wdog/wdog.c

# include directories
INCLUDE_DIRS  += $(NV32_COMMON_DIR)

INCLUDE_DIRS  += $(NV32_CPU_LIB)
INCLUDE_DIRS  += $(NV32_CPU_LIB)/headers
INCLUDE_DIRS  += $(NV32_PLATFORM_DIR)

INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/acmp
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/adc
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/crc
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/gpio
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/ics
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/iic
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/kbi
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/eeprom
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/nvm
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/pit
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/PMC
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/rtc
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/sim
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/spi
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/uart
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/etm
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/delay
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/wdog
INCLUDE_DIRS  += $(NV32_DRIVER_LIB)/bos
