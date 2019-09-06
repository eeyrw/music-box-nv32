/******************************************************************************
*
* NOTE:系统所使用的一些宏定义，以及时钟模式的选择。
*
******************************************************************************/

#ifndef _NVxx_CONFIG_H_
#define _NVxx_CONFIG_H_

#include <stdint.h>

#define CPU_NV32
#define TEST


//#define TRIM_IRC                /*!< 是否使用定义的TRIM值来校准内部IRC，若注释则使用出厂校准的TRIM值出厂校准至31.25K-1280倍频->40M */ //Modify
#define SPI0_PINREMAP             /*!< SPI0的管脚映射定义 */
#define ENABLE_WDOG               /*!< 禁用看门狗，但允许其更新 */
#define DISABLE_NMI               /*!< 禁用NMI中断输入引脚-PB4,该引脚建议布板外接4.7K-10K的上拉，以及上电时该引脚不可为低(若不使用NMI功能)*/

/*! 定义是否打印系统信息 */      
#define PRINT_SYS_LOG           

//#define OUTPUT_BUSCLK           /*!< 定义是否输出系统时钟，输出引脚为PH2 */
#define ICS_TRIM_VALUE  0x50       

/*****************************************************************************
 *
 * 根据所选的时钟模式，及时钟分频系数确定最终的总线时钟BUSCLK和系统/内核时钟ICSOUT
 * FEI模式：内核/系统时钟=IRC*1280/BDIV ：总线时钟=IRC*1280/BDIV/BUSDIV
 * FEE模式：内核/系统时钟=晶振/RDIV*1280/BDIV :总线时钟=晶振/RDIV*1280/BDIV/BUSDIV
 * 其他时钟模式频率的计算请参考：库函数说明及参考手册ICS部分
 *
 ******************************************************************************/

/*! 定义时钟的时钟模式以及频率 */
//#define USE_FEE                         /*!< 使用外部时钟FEE模式 */
//#define USE_FEE_OSC                     /*!< 使用外部时钟输入OSC模式 */
  #define USE_FEI                         /*!< 使用系统内部时钟作为主频 */
//#define USE_FBELP	
//#define USE_FBE_OSC

/*! 定义BDIV分频系数 (BDIV) */
/*
 * BDIV_ENCODE ：  0  1  2  3  4   5   6    7
 *
 * BDIV分频系数：  1  2  4  8  16  32  64  128
 */
#define BDIV_ENCODE      0 							/*!< BDIV分频系数为1 */

/*! 定义BUSDIV分频系数(BUSDIV) */
/* 
 * BUSDIV_ENCODE     0   1
 *
 * BUSCLK=ICSOUT/x   1   2
 */
//此处建议不要修改，即系统/内核时钟=总线时钟
#define BUSDIV_ENCODE        0        /*总线时钟=ICSOUT/1*/ 


/*! 若使用外部晶振，则需要定义外部晶振频率. */
//#define EXT_CLK_FREQ_KHZ	32          /* in KHz */
//#define EXT_CLK_FREQ_KHZ	4000        /* in KHz */
//#define EXT_CLK_FREQ_KHZ	8000        /* in KHz */
  #define EXT_CLK_FREQ_KHZ	10000       /* in KHz */	
//#define EXT_CLK_FREQ_KHZ	16000       /* in KHz */
//#define EXT_CLK_FREQ_KHZ	20000       /* in KHz */


//例：使用FEI模式： 内核时钟/系统时钟 = ICSOUT=31.25K*1280/1=40M， 总线时钟 = BUSCLK = ICSOUT/1 = 40M           

/* 定义总线时钟主频 */
//一、前提：若定义使用FEI内部时钟
#if	defined(USE_FEI)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV分频系数为1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV分频系数为2*/
	//#define  BUS_CLK_HZ		10000000L     //#define BDIV_ENCODE      2 /*BDIV分频系数为4*/
//二、前提：若定义使用外部时钟的模式，以下情况为使用通用FEE模式，关于其他模式的使用参见手册及库函数说明
#elif  (EXT_CLK_FREQ_KHZ == 10000)
    #define  BUS_CLK_HZ		25000000L     //#define BDIV_ENCODE      1 /*BDIV分频系数为2*/ //NV32F100x的主频需在40M以内
#elif  (EXT_CLK_FREQ_KHZ == 16000)
    #define  BUS_CLK_HZ   25000000L     //#define BDIV_ENCODE      1 /*BDIV分频系数为2*/ //NV32F100x的主频需在40M以内
#elif  (EXT_CLK_FREQ_KHZ == 8000)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV分频系数为1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV分频系数为2*/
#elif  (EXT_CLK_FREQ_KHZ == 4000)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV分频系数为1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV分频系数为2*/
#elif  (EXT_CLK_FREQ_KHZ == 32)
    #define  BUS_CLK_HZ		16777216L
//三、定义了其他模式
#else   
    #define  BUS_CLK_HZ		40000000L
#endif

/*! 定义所使用UART口的波特率 */
#define UART_PRINT_BITRATE              115200      /*! UART波特率 */
/*! 定义所使用的UART口 */
#define TERM_PORT   UART0              						  /*!< 定义使用UART1口，开发板上默认使用UART1口 */

/*! 板载LED */
#define LED0_Init()            	GPIOB->PDDR |= (1<<25)  //初始化
#define LED0_Toggle()           GPIOB->PTOR  = (1<<25)  //翻转
#define LED0_On()               GPIOB->PCOR  = (1<<25)  //置位
#define LED0_Off()              GPIOB->PSOR  = (1<<25)  //清零

#define LED1_Init()            	GPIOB->PDDR |= (1<<26)  
#define LED1_Toggle()           GPIOB->PTOR  = (1<<26)
#define LED1_On()               GPIOB->PCOR  = (1<<26)
#define LED1_Off()              GPIOB->PSOR  = (1<<26)

#define LED2_Init()            	GPIOB->PDDR |= (1<<7)     
#define LED2_Toggle()           GPIOB->PTOR  = (1<<7)
#define LED2_On()               GPIOB->PCOR  = (1<<7)
#define LED2_Off()              GPIOB->PSOR  = (1<<7)

#endif /* NVxx_CONFIG_H_ */
