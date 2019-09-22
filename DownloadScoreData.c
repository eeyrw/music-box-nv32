#include <stdint.h>
#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"
#include "flash.h"
#include "eeprom.h"

/* application address */
#define BOOT_ADDR 24 * 1024 //24k

/* flash block size */
#define BLOCK_SIZE 512

/* not configured by user */
#define STR(x) #x
#define STRX(x) STR(x)

static uint8_t CRC;
static uint8_t rx_buffer[BLOCK_SIZE];

/**
 * Initialize UART1 in 8N1 mode
 */
void uart_init()
{

    UART_ConfigType sConfig;

    sConfig.u32SysClkHz = BUS_CLK_HZ;         //选择时钟源为总线时钟
    sConfig.u32Baudrate = UART_PRINT_BITRATE; //设置波特率
    UART_Init(UART0, &sConfig); //初始化串口 1
    SIM_RemapUART0Pin();
}

/**
 * Write byte into UART
 */
void uart_write(uint8_t data)
{
    UART_PutChar(UART0, data);
}

/**
 * Read byte from UART and reset watchdog
 */
uint8_t uart_read()
{
    return UART_GetChar(UART0);
}

/**
 * Calculate CRC-8-CCIT.
 * Polynomial: x^8 + x^2 + x + 1 (0x07)
 *
 * @param data input byte
 * @param crc initial CRC
 * @return CRC value
 */
uint8_t crc8_update(uint8_t data, uint8_t crc)
{
    crc ^= data;
    for (uint8_t i = 0; i < 8; i++)
        crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : crc << 1;
    return crc;
}

/**
 * Send ACK response
 */
void serial_send_ack()
{
    uart_write(0xAA);
    uart_write(0xBB);
}

/**
 * Send NACK response (CRC mismatch)
 */
void serial_send_nack()
{
    uart_write(0xDE);
    uart_write(0xAD);
}

/**
 * Read BLOCK_SIZE bytes from UART
 *
 * @param dest destination buffer
 */
void serial_read_block(uint8_t *dest)
{
    serial_send_ack();
    for (uint32_t i = 0; i < BLOCK_SIZE; i++)
    {
        uint8_t rx = uart_read();
        dest[i] = rx;
        CRC = crc8_update(rx, CRC);
    }
}

/**
 * Enter bootloader and perform firmware update
 */
void bootloader_exec()
{
    uint8_t chunks, crc_rx;
    uint32_t addr = BOOT_ADDR;

    /* enter bootloader */
    for (;;)
    {
        uint8_t rx = uart_read();
        if (rx != 0xDE)
            continue;
        rx = uart_read();
        if (rx != 0xAD)
            continue;
        rx = uart_read();
        if (rx != 0xBE)
            continue;
        rx = uart_read();
        if (rx != 0xEF)
            continue;
        chunks = uart_read();
        crc_rx = uart_read();
        rx = uart_read();
        if (crc_rx != rx)
            continue;
        break;
    }

    /* unlock flash */
    Flash_Init();
    /* get main firmware */
    for (uint32_t i = 0; i < chunks; i++)
    {
        serial_read_block(rx_buffer);
        Flash_EraseSector(addr);
        Flash_Program(addr, rx_buffer, BLOCK_SIZE);
        addr += BLOCK_SIZE;
    }

    /* verify CRC */
    if (CRC != crc_rx)
    {
        serial_send_nack();
        for (;;)
            ;
    }

    serial_send_ack();
}

void bootloader_main()
{

    /* execute bootloader */
    uart_init();
    bootloader_exec();
}