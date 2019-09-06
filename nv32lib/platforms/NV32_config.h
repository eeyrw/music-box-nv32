/******************************************************************************
*
* NOTE:ϵͳ��ʹ�õ�һЩ�궨�壬�Լ�ʱ��ģʽ��ѡ��
*
******************************************************************************/

#ifndef _NVxx_CONFIG_H_
#define _NVxx_CONFIG_H_

#include <stdint.h>

#define CPU_NV32
#define TEST


//#define TRIM_IRC                /*!< �Ƿ�ʹ�ö����TRIMֵ��У׼�ڲ�IRC����ע����ʹ�ó���У׼��TRIMֵ����У׼��31.25K-1280��Ƶ->40M */ //Modify
#define SPI0_PINREMAP             /*!< SPI0�Ĺܽ�ӳ�䶨�� */
#define ENABLE_WDOG               /*!< ���ÿ��Ź�������������� */
#define DISABLE_NMI               /*!< ����NMI�ж���������-PB4,�����Ž��鲼�����4.7K-10K���������Լ��ϵ�ʱ�����Ų���Ϊ��(����ʹ��NMI����)*/

/*! �����Ƿ��ӡϵͳ��Ϣ */      
#define PRINT_SYS_LOG           

//#define OUTPUT_BUSCLK           /*!< �����Ƿ����ϵͳʱ�ӣ��������ΪPH2 */
#define ICS_TRIM_VALUE  0x50       

/*****************************************************************************
 *
 * ������ѡ��ʱ��ģʽ����ʱ�ӷ�Ƶϵ��ȷ�����յ�����ʱ��BUSCLK��ϵͳ/�ں�ʱ��ICSOUT
 * FEIģʽ���ں�/ϵͳʱ��=IRC*1280/BDIV ������ʱ��=IRC*1280/BDIV/BUSDIV
 * FEEģʽ���ں�/ϵͳʱ��=����/RDIV*1280/BDIV :����ʱ��=����/RDIV*1280/BDIV/BUSDIV
 * ����ʱ��ģʽƵ�ʵļ�����ο����⺯��˵�����ο��ֲ�ICS����
 *
 ******************************************************************************/

/*! ����ʱ�ӵ�ʱ��ģʽ�Լ�Ƶ�� */
//#define USE_FEE                         /*!< ʹ���ⲿʱ��FEEģʽ */
//#define USE_FEE_OSC                     /*!< ʹ���ⲿʱ������OSCģʽ */
  #define USE_FEI                         /*!< ʹ��ϵͳ�ڲ�ʱ����Ϊ��Ƶ */
//#define USE_FBELP	
//#define USE_FBE_OSC

/*! ����BDIV��Ƶϵ�� (BDIV) */
/*
 * BDIV_ENCODE ��  0  1  2  3  4   5   6    7
 *
 * BDIV��Ƶϵ����  1  2  4  8  16  32  64  128
 */
#define BDIV_ENCODE      0 							/*!< BDIV��Ƶϵ��Ϊ1 */

/*! ����BUSDIV��Ƶϵ��(BUSDIV) */
/* 
 * BUSDIV_ENCODE     0   1
 *
 * BUSCLK=ICSOUT/x   1   2
 */
//�˴����鲻Ҫ�޸ģ���ϵͳ/�ں�ʱ��=����ʱ��
#define BUSDIV_ENCODE        0        /*����ʱ��=ICSOUT/1*/ 


/*! ��ʹ���ⲿ��������Ҫ�����ⲿ����Ƶ��. */
//#define EXT_CLK_FREQ_KHZ	32          /* in KHz */
//#define EXT_CLK_FREQ_KHZ	4000        /* in KHz */
//#define EXT_CLK_FREQ_KHZ	8000        /* in KHz */
  #define EXT_CLK_FREQ_KHZ	10000       /* in KHz */	
//#define EXT_CLK_FREQ_KHZ	16000       /* in KHz */
//#define EXT_CLK_FREQ_KHZ	20000       /* in KHz */


//����ʹ��FEIģʽ�� �ں�ʱ��/ϵͳʱ�� = ICSOUT=31.25K*1280/1=40M�� ����ʱ�� = BUSCLK = ICSOUT/1 = 40M           

/* ��������ʱ����Ƶ */
//һ��ǰ�᣺������ʹ��FEI�ڲ�ʱ��
#if	defined(USE_FEI)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV��Ƶϵ��Ϊ1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV��Ƶϵ��Ϊ2*/
	//#define  BUS_CLK_HZ		10000000L     //#define BDIV_ENCODE      2 /*BDIV��Ƶϵ��Ϊ4*/
//����ǰ�᣺������ʹ���ⲿʱ�ӵ�ģʽ���������Ϊʹ��ͨ��FEEģʽ����������ģʽ��ʹ�òμ��ֲἰ�⺯��˵��
#elif  (EXT_CLK_FREQ_KHZ == 10000)
    #define  BUS_CLK_HZ		25000000L     //#define BDIV_ENCODE      1 /*BDIV��Ƶϵ��Ϊ2*/ //NV32F100x����Ƶ����40M����
#elif  (EXT_CLK_FREQ_KHZ == 16000)
    #define  BUS_CLK_HZ   25000000L     //#define BDIV_ENCODE      1 /*BDIV��Ƶϵ��Ϊ2*/ //NV32F100x����Ƶ����40M����
#elif  (EXT_CLK_FREQ_KHZ == 8000)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV��Ƶϵ��Ϊ1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV��Ƶϵ��Ϊ2*/
#elif  (EXT_CLK_FREQ_KHZ == 4000)
    #define  BUS_CLK_HZ		40000000L     //#define BDIV_ENCODE      0 /*BDIV��Ƶϵ��Ϊ1*/
	//#define  BUS_CLK_HZ		20000000L     //#define BDIV_ENCODE      1 /*BDIV��Ƶϵ��Ϊ2*/
#elif  (EXT_CLK_FREQ_KHZ == 32)
    #define  BUS_CLK_HZ		16777216L
//��������������ģʽ
#else   
    #define  BUS_CLK_HZ		40000000L
#endif

/*! ������ʹ��UART�ڵĲ����� */
#define UART_PRINT_BITRATE              115200      /*! UART������ */
/*! ������ʹ�õ�UART�� */
#define TERM_PORT   UART0              						  /*!< ����ʹ��UART1�ڣ���������Ĭ��ʹ��UART1�� */

/*! ����LED */
#define LED0_Init()            	GPIOB->PDDR |= (1<<25)  //��ʼ��
#define LED0_Toggle()           GPIOB->PTOR  = (1<<25)  //��ת
#define LED0_On()               GPIOB->PCOR  = (1<<25)  //��λ
#define LED0_Off()              GPIOB->PSOR  = (1<<25)  //����

#define LED1_Init()            	GPIOB->PDDR |= (1<<26)  
#define LED1_Toggle()           GPIOB->PTOR  = (1<<26)
#define LED1_On()               GPIOB->PCOR  = (1<<26)
#define LED1_Off()              GPIOB->PSOR  = (1<<26)

#define LED2_Init()            	GPIOB->PDDR |= (1<<7)     
#define LED2_Toggle()           GPIOB->PTOR  = (1<<7)
#define LED2_On()               GPIOB->PCOR  = (1<<7)
#define LED2_Off()              GPIOB->PSOR  = (1<<7)

#endif /* NVxx_CONFIG_H_ */
