#include <stdint.h>
#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"
#include "flash.h"
#include "eeprom.h"
#include "ringbuffer.h"
#include "DownloadScoreData.h"

#define EVENT_FRAME_FLAG 0x776E //ASCII:"wn"

#define CMD_FLASH_INIT 0x01
#define CMD_FLASH_WRITE_BLOCK 0x02
#define CMD_FLASH_DEINIT 0x03

#define CMD_OFFEST 0x03

typedef enum _EVENT_FRAME_PARSER_STATUS {
	FRAME_PARSER_STATUS_IDLE = 0,
	FRAME_PARSER_STATUS_SOF_LO,
	FRAME_PARSER_STATUS_SOF_HI,
	FRAME_PARSER_STATUS_RECV_CMD_LEN,

} EVENT_FRAME_PARSER_STATUS;

EVENT_FRAME_PARSER_STATUS frameParseStatus=FRAME_PARSER_STATUS_IDLE;

/* application address */
#define BOOT_ADDR 24 * 1024 //24k

/* flash block size */
#define BLOCK_SIZE 512

ring_buffer_t ring_buffer;

uint8_t cmdBuf[1024];
uint8_t cmdRetBuf[1024];


void UART_HandleInt(UART_Type *pUART)
{

    volatile uint8_t read_temp = 0;

    /* �������Ƿ���� */
    if(UART_CheckFlag(pUART,UART_FlagOR))
    {
        read_temp = UART_ReadDataReg(pUART);
    }

    /* ���������Ƿ��� */
    else if(UART_IsRxBuffFull(pUART))
    {
        /* �������ݵĻ����� */
        read_temp = UART_ReadDataReg(pUART);
        ring_buffer_queue(&ring_buffer,read_temp);
    }

}

/**
 * Initialize UART1 in 8N1 mode
 */
void uart_init()
{

    UART_ConfigType sConfig;

    sConfig.u32SysClkHz = BUS_CLK_HZ;         //ѡ��ʱ��ԴΪ����ʱ��
    sConfig.u32Baudrate = UART_PRINT_BITRATE; //���ò�����
    UART_Init(UART0, &sConfig); //��ʼ������ 1
    UART_SetCallback(UART_HandleInt);
    UART_EnableInterrupt(UART0,UART_RxBuffFullInt);
    UART_EnableInterrupt(UART0,UART_RxOverrunInt);
    SIM_RemapUART0Pin();
    NVIC_EnableIRQ(UART0_IRQn);
}




void DownloadInit()
{

    /* execute bootloader */
    uart_init();
    /* Create and initialize ring buffer */

    ring_buffer_init(&ring_buffer);
}

uint8_t *GetCmdDataPtr(uint8_t *buffer)
{
	return buffer + CMD_OFFEST;
}



int Protocol_Process(unsigned char *Buf)
{
	unsigned int i;
	int retByteNum = 0;
	uint8_t *rbf;
	uint16_t blockIndex;
	uint16_t blockSize;

	switch (Buf[0])
	{

	case CMD_FLASH_WRITE_BLOCK:
		blockIndex =  Buf[1]|Buf[2]<<8;
		blockSize =  Buf[3]|Buf[4]<<8;
        //Flash_EraseSector(blockIndex*BLOCK_SIZE+BOOT_ADDR);
        //Flash_Program(blockIndex*BLOCK_SIZE+BOOT_ADDR, &Buf[5], blockSize);


		rbf = GetCmdDataPtr(cmdRetBuf);
		rbf[0] = CMD_FLASH_WRITE_BLOCK;
		rbf[1] = 4;
		rbf[2] = Buf[1];
		rbf[3] = Buf[2];
		rbf[4] = Buf[3];
		rbf[5] = Buf[4];
		retByteNum = rbf[1] + 1 + 1;
		break;

	default:
		break;
	}
	return retByteNum;
}


void ParseEventFrameStream(ring_buffer_t* buffer)
{
	uint8_t streamByte;
	static uint8_t cmdLen = 0;

	switch (frameParseStatus)
	{
	case FRAME_PARSER_STATUS_IDLE:
	{
		if (!ring_buffer_is_empty(buffer))
		{
			ring_buffer_dequeue(buffer,&streamByte);
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
			ring_buffer_dequeue(buffer,&streamByte);
			if (streamByte == ((uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8))))
			{
				frameParseStatus = FRAME_PARSER_STATUS_SOF_HI;
			}
		}
	}
	break;
	case FRAME_PARSER_STATUS_SOF_HI:
	{
		if (!ring_buffer_is_empty(buffer))
		{
			ring_buffer_dequeue(buffer,&streamByte);
			cmdLen = streamByte;
			frameParseStatus = FRAME_PARSER_STATUS_RECV_CMD_LEN;
		}
	}
	break;

	case FRAME_PARSER_STATUS_RECV_CMD_LEN:
	{
		if (ring_buffer_num_items(buffer)>= cmdLen)
		{
			ring_buffer_dequeue_arr(buffer,cmdBuf, cmdLen);
			int retByteNum = Protocol_Process(cmdBuf);
			if (retByteNum > 0)
			{
				cmdRetBuf[0] = (uint8_t)(0xFF & EVENT_FRAME_FLAG);
				cmdRetBuf[1] = (uint8_t)(0xFF & (EVENT_FRAME_FLAG >> 8));
				cmdRetBuf[2] = retByteNum;
				retByteNum += 3;
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
