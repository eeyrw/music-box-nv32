/*
 * DownloadScoreData.h
 *
 *  Created on: 2019Äê9ÔÂ23ÈÕ
 *      Author: yuan
 */

#ifndef DOWNLOADSCOREDATA_H_
#define DOWNLOADSCOREDATA_H_

#include <stdint.h>
#include "ringbuffer.h"



/**
 * Initialize UART1 in 8N1 mode
 */
void uart_init(void);

void DownloadInit(void);
uint8_t *GetCmdDataPtr(uint8_t *buffer);
int Protocol_Process(unsigned char *Buf);
void ParseEventFrameStream(ring_buffer_t* buffer);

void DownloadProcess(void);

#endif /* DOWNLOADSCOREDATA_H_ */
