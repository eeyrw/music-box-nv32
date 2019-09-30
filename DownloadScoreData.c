#include <stdint.h>
#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"
#include "flash.h"
#include "eeprom.h"
#include "ringbuffer.h"
#include "DownloadScoreData.h"
#include "Player.h"
#include <string.h>

#define EVENT_FRAME_FLAG 0x776E //ASCII:"wn"

enum _CMD
{
	CMD_FLASH_WRITE_START = 0x01,
	CMD_FLASH_WRITE_BLOCK,
	CMD_FLASH_WRITE_END,
	CMD_FLASH_SZIE
};

#define CMD_OFFEST 0x04

typedef struct _CMD_FLASH_WRITE_BLOCK_HEADER
{
	uint16_t blockIndex;
	uint16_t dataLen;
} __attribute__((packed)) CMD_FLASH_WRITE_BLOCK_HEADER;

typedef enum _EVENT_FRAME_PARSER_STATUS
{
	FRAME_PARSER_STATUS_IDLE = 0,
	FRAME_PARSER_STATUS_SOF_LO,
	FRAME_PARSER_STATUS_SOF_HI,
	FRAME_PARSER_STATUS_RECV_CMD_LEN,

} EVENT_FRAME_PARSER_STATUS;

EVENT_FRAME_PARSER_STATUS frameParseStatus = FRAME_PARSER_STATUS_IDLE;

/* application address */
#define BOOT_ADDR 24 * 1024 //24k

/* flash block size */
#define BLOCK_SIZE 512

ring_buffer_t ring_buffer;

uint8_t cmdBuf[1024];
uint8_t cmdRetBuf[1024];

uint8_t blockBuf[BLOCK_SIZE];

extern uint32_t __SCORE_DATA_FLASH_SIZE__;
extern Player mPlayer;

void UART_HandleInt(UART_Type *pUART)
{

	volatile uint8_t read_temp = 0;

	/* 检查接收是否过载 */
	if (UART_CheckFlag(pUART, UART_FlagOR))
	{
		read_temp = UART_ReadDataReg(pUART);
	}
	/* 检测接收器是否满 */
	else if (UART_IsRxBuffFull(pUART))
	{
		/* 接收数据的缓冲区 */
		read_temp = UART_ReadDataReg(pUART);
		ring_buffer_queue(&ring_buffer, read_temp);
	}
}

/**
 * Initialize UART1 in 8N1 mode
 */
void uart_init()
{

	UART_ConfigType sConfig;

	sConfig.u32SysClkHz = BUS_CLK_HZ; //选择时钟源为总线时钟
	sConfig.u32Baudrate = 115200;	 //设置波特率
	UART_Init(UART0, &sConfig);		  //初始化串口 1
	UART_SetCallback(UART_HandleInt);
	UART_EnableInterrupt(UART0, UART_RxBuffFullInt);
	UART_EnableInterrupt(UART0, UART_RxOverrunInt);
	SIM_RemapUART0Pin();
	NVIC_EnableIRQ(UART0_IRQn);
}

void DownloadInit()
{
	uart_init();
	ring_buffer_init(&ring_buffer);
	Flash_Init();
}

uint8_t *GetCmdDataPtr(uint8_t *buffer)
{
	return buffer + CMD_OFFEST;
}

int Protocol_Process(unsigned char *Buf)
{
	int retByteNum = 0;
	uint8_t *rbf;
	uint32_t fSize = (uint32_t)(&__SCORE_DATA_FLASH_SIZE__);

	CMD_FLASH_WRITE_BLOCK_HEADER *cmdFlashWriteBlockHeader;

	switch (Buf[0])
	{
	case CMD_FLASH_WRITE_START:
		StopPlayScheduler(&mPlayer);
		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = Buf[0];
		retByteNum = 1;
		break;

	case CMD_FLASH_WRITE_END:
		StartPlayScheduler(&mPlayer);
		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = Buf[0];
		retByteNum = 1;
		break;

	case CMD_FLASH_WRITE_BLOCK:
		cmdFlashWriteBlockHeader = (CMD_FLASH_WRITE_BLOCK_HEADER *)&Buf[1];
		uint32_t addr = cmdFlashWriteBlockHeader->blockIndex * BLOCK_SIZE + BOOT_ADDR;
		Flash_EraseSector(addr);
		memcpy(blockBuf, &Buf[5], cmdFlashWriteBlockHeader->dataLen);
		Flash_Program(addr, blockBuf, cmdFlashWriteBlockHeader->dataLen);

		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = Buf[0];
		rbf[1] = Buf[1];
		rbf[2] = Buf[2];
		rbf[3] = Buf[3];
		rbf[4] = Buf[4];
		retByteNum = 5;
		break;

	case CMD_FLASH_SZIE:
		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = Buf[0];
		rbf[1] = fSize & 0xFF;
		rbf[2] = (fSize >> 8) & 0xFF;
		rbf[3] = (fSize >> 16) & 0xFF;
		rbf[4] = (fSize >> 24) & 0xFF;
		retByteNum = 5;
		break;

	default:
		break;
	}
	return retByteNum;
}

void ParseEventFrameStream(ring_buffer_t *buffer)
{
	uint8_t streamByte;
	static int cmdLen = 0;

	switch (frameParseStatus)
	{
	case FRAME_PARSER_STATUS_IDLE:
	{
		if (!ring_buffer_is_empty(buffer))
		{
			ring_buffer_dequeue(buffer, &streamByte);
			if (streamByte == ((uint8_t)(0xFF & EVENT_FRAME_FLAG)))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_LO;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_LO:
	{
		if (!ring_buffer_is_empty(buffer))
		{
			ring_buffer_dequeue(buffer, &streamByte);
			if (streamByte == ((uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8))))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_HI;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_HI:
	{
		if (ring_buffer_num_items(buffer) >= 2)
		{
			ring_buffer_dequeue(buffer, &streamByte);
			cmdLen = streamByte;
			ring_buffer_dequeue(buffer, &streamByte);
			cmdLen += streamByte << 8;
			frameParseStatus = FRAME_PARSER_STATUS_RECV_CMD_LEN;
		}
	}
	break;

	case FRAME_PARSER_STATUS_RECV_CMD_LEN:
	{
		int bufferRemain = ring_buffer_num_items(buffer);
		static int receiveCounter = 0;
		if (receiveCounter < cmdLen)
		{
			if (bufferRemain > 0)
			{
				if (bufferRemain + receiveCounter > cmdLen)
					bufferRemain = cmdLen - receiveCounter;
				ring_buffer_dequeue_arr(buffer, cmdBuf + receiveCounter, bufferRemain);
				receiveCounter += bufferRemain;
			}
		}
		else
		{
			receiveCounter = 0;
			int retByteNum = Protocol_Process(cmdBuf);
			if (retByteNum > 0)
			{
				cmdRetBuf[0] = (uint8_t)(0xFF & EVENT_FRAME_FLAG);
				cmdRetBuf[1] = (uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8));
				cmdRetBuf[2] = retByteNum & 0xFF;
				cmdRetBuf[3] = retByteNum >> 8;
				retByteNum += 4;
				UART_SendWait(UART0, cmdRetBuf, retByteNum);
			}

			frameParseStatus = FRAME_PARSER_STATUS_IDLE;
			cmdLen = 0;
		}
	}
	break;

	default:
		break;
	}
}

void DownloadProcess(void)
{
	ParseEventFrameStream(&ring_buffer);
}
