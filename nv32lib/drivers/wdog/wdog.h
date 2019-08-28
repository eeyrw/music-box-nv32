/******************************************************************************
*
* @brief  WDG ����ͷ�ļ�.  
*
******************************************************************************/

#ifndef __WDOG_H__
#define __WDOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "sim.h"
/*****************************************************************************/
/******************************************************************************
* ����WDGʱ��Դ
*
*//*! @WDGʱ��Դ
* @{
*******************************************************************************/

#define WDOG_CLK_BUS                    0   /*!< ����ʱ�� */
#define WDOG_CLK_INTERNAL_32KHZ         2   /*!< �ڲ� 32 kHz (ICSIRCLK) */
#define WDOG_CLK_INTERNAL_1KHZ	        1   /*!< �ڲ� LPO 1 KHz */
#define WDOG_CLK_EXTERNAL               3   /*!< �ⲿʱ��  */
/*! @}                                					*/

/* ���Ź�ʱ��Դѡ�� */
#define WDOG_CLK  (WDOG_CLK_INTERNAL_1KHZ)
    
/* ���Ź��Ĵ���Ĭ��ֵ���� */
#define WDOG_CS1_DEFAULT_VALUE      0x80
#define WDOG_CS2_DEFAULT_VALUE      0x01
#define WDOG_TOVAL_DEFAULT_VALUE    0x0400
#define WDOG_WIN_DEFAULT_VALUE      0x0000
     

/*! 
 * @brief ���Ź�����.
 */
#define WDOG_Unlock()       DisableInterrupts; WDOG->CNT = 0x20C5; WDOG->CNT = 0x28D9; EnableInterrupts       //Modify 
/*! @}                            					*/

/******************************************************************************
* define ���Ź����ýṹ��
*
*//*! @���Ź����ýṹ��
* @{
*******************************************************************************/
typedef struct {	
    struct {
	uint16_t    bIntEnable      : 1;    /*!< ���Ź��ж�ʹ�� */
	uint16_t    bDisable        : 1;    /*!< ���ÿ��Ź� */
	uint16_t    bWaitEnable     : 1;    /*!< ʹ�ܿ��Ź��ȴ�ģʽ */
	uint16_t    bStopEnable     : 1;    /*!< ʹ�ܿ��Ź�ֹͣģʽ */
	uint16_t    bDbgEnable      : 1;    /*!< ʹ�ܿ��Ź�����ģʽ */
	uint16_t    bWinEnable      : 1;    /*!< ʹ�ܿ��Ź�����ģʽ */
	uint16_t    bUpdateEnable   : 1;    /*!< ʹ�������Ź����� */
	uint16_t    bClkSrc         : 2;    /*!< ���Ź�ʱ��Դѡ�� */  
	uint16_t    bPrescaler      : 1;    /*!< Ԥ��Ƶϵ�� */  
    }sBits;                             /*!< λ��ṹ��  */
    uint16_t    u16ETMeOut;             /*!< ���Ź���ʱֵ */
    uint16_t    u16WinETMe;             /*!< ���Ź�����ֵ */
} WDOG_ConfigType, *WDOG_ConfigPtr;  /*!< ���Ź����ýṹ������ */
/*! @}                               					*/

/******************************************************************************
******************************************************************************/

/*!
 * ��������
 */

/*****************************************************************************//*!
*
* @brief ���ÿ��Ź���ʱֵ.
*        
* @param[in]   u16ETMeOut    ��ʱֵ.
*
* @return none
*
*****************************************************************************/

__STATIC_INLINE void WDOG_SetETMeOut(uint16_t u16ETMeOut)
{
    WDOG->CNT = 0x20C5; 
    WDOG->CNT = 0x28D9;
    WDOG->TOVAL8B.TOVALL  = u16ETMeOut;
    WDOG->TOVAL8B.TOVALH  = u16ETMeOut >> 8;
}


/*****************************************************************************//*!
*
* @brief ���ÿ��Ź�����.
*        
* @param[in]   u16WinETMe    ���Ź�����ֵ.
*
* @return none
*
*****************************************************************************/

__STATIC_INLINE void WDOG_SetWindow(uint16_t u16WinETMe)
{
    WDOG->CNT = 0x20C5; 
    WDOG->CNT = 0x28D9;
    WDOG->WIN8B.WINL  = u16WinETMe;
    WDOG->WIN8B.WINH  = u16WinETMe >> 8;
}

/*****************************************************************************//*!
*
* @brief ��鿴�Ź��Ƿ�λ.
*        
* @param    none.
*
* @return   TRUE �����Ź���λ����, FALSE ��.
*
*****************************************************************************/

__STATIC_INLINE uint8_t WDOG_IsReset(void)
{
    if(SIM_GetStatus(SIM_SRSID_WDOG_MASK))
    {
        return (TRUE);
    }
    return (FALSE);
}

/*! @}                                       					*/


void WDOG_Init(WDOG_ConfigPtr pConfig);
void WDOG_DeInit(void);
void WDOG_Disable(void);
void WDOG_DisableWDOGEnableUpdate(void);
void WDOG_Enable(void);
void WDOG_Feed(void);
void WDOG_SetETMeOut(uint16_t u16ETMeOut);
void WDOG_SetWindow(uint16_t u16WinETMe);
void WDOG_EnableUpdate(void);
void WDOG_DisableUpdate(void);
uint8_t WDOG_IsReset(void);

#ifdef __cplusplus
}
#endif

/********************************************************************/
#endif /* __WDOG_H__ */

