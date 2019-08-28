/******************************************************************************
*
* @brief  RTC ����ͷ�ļ�.  
*
******************************************************************************/
#ifndef RTC_H_
#define RTC_H_
#ifdef __cplusplus
extern "C" {
#endif
	
#include "common.h"
/******************************************************************************
* RTC����λ����
*
*//*! 
* @{
*******************************************************************************/

#define RTC_OUTPUT_ENABLE           1	/*!< ʹ��RTC�������  */ 
#define RTC_INTERRUPT_ENABLE        1	/*!< ʹ��RTC�ж�  */ 
#define RTC_CLKSRC_EXTERNAL         0	/*!< ѡ���ⲿʱ����ΪRTCʱ��Դ  */
#define RTC_CLKSRC_1KHZ             1	/*!< ѡ��LPOʱ����ΪRTCʱ��Դ  */
#define RTC_CLKSRC_IREF             2	/*!< ѡ���ڲ��ο�ʱ��ICSIRCLK��ΪRTCʱ��Դ  */
#define RTC_CLKSRC_BUS              3	/*!< ѡ������ʱ����ΪRTCʱ��Դ  */
#define RTC_CLK_PRESCALER_128       1	/*!< ����RTCLKSλѡ����1����128��Ƶ */
#define RTC_CLK_PRESCALER_256       2	/*!< ����RTCLKSλѡ����2����256��Ƶ */
#define RTC_CLK_PRESCALER_512       3	/*!< ����RTCLKSλѡ����4����512��Ƶ */
#define RTC_CLK_PRESCALER_1024      4	/*!< ����RTCLKSλѡ����8����1024��Ƶ */
#define RTC_CLK_PRESCALER_2048      5	/*!< ����RTCLKSλѡ����16����2048��Ƶ */
#define RTC_CLK_PRESCALER_100       6	/*!< ����RTCLKSλѡ����32����100��Ƶ */
#define RTC_CLK_PRESCALER_1000      7	/*!< ����RTCLKSλѡ����64����1000��Ƶ */


/*! @}                                                                        */


/*!
 * @brief RTC �ص�����.
 *
 */

typedef void (*RTC_CallbackType)(void);



/* RTC ���ýṹ����
 */  
/*!
 * @brief RTC ���ò���.
 *
 */
typedef struct
{   
    uint16_t bReserved                  : 4;    /*!< ���� */     
    uint16_t bRTCOut                    : 1;    /*!< 1: ʹ��RTC���, 0: ��ֹRTC��� */
    uint16_t bReserved1                 : 1;    /*!< ���� */ 
    uint16_t bInterruptEn               : 1;    /*!< 1: ʹ��RTC�ж�, 0: RTC ��ֹRTC�ж� */
    uint16_t bFlag                      : 1;    /*!< 1: RTCʵʱ�жϱ�־ */   
    uint16_t bClockPresaler             : 3;    /*!< 3: RTC��Ƶϵ��ѡ�� */    
    uint16_t bReserved2                 : 3;    /*!< ����*/ 
    uint16_t bClockSource               : 2;    /*!< 2:RTCʱ��Դѡ�� */
    uint16_t u16ModuloValue                ;    /*!< 16λRTCģֵ */
} RTC_ConfigType, *RTC_ConfigPtr; 


/******************************************************************************
******************************************************************************/

/*!
 * ��������
 */
/*****************************************************************************//*!
*
* @brief ʹ��RTC�ж�.
*        
* @param none
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void RTC_EnableInt(void)
{
    RTC->SC |= RTC_SC_RTIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ��ֹRTC�ж�.
*        
* @param none
*
* @return non
*
*****************************************************************************/
__STATIC_INLINE void RTC_DisableInt(void)
{
    RTC->SC &= ~RTC_SC_RTIE_MASK;
}

/*****************************************************************************//*!
*
* @brief ����16λRTCģֵ.
*        
* @param[in]   u16Mod_Value   16λģֵ
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void RTC_SetModulo(uint16_t u16Mod_Value)
{
 
    RTC->MOD = u16Mod_Value;
}

/*****************************************************************************//*!
*
* @brief ����RTCʱ���Լ���Ƶϵ��.
*        
* @param[in]   u16Clock_Number   ѡ��ʱ��Դ
* @param[in]   u16Presalcer      ��Ƶϵ��
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void RTC_SetClock(uint16_t u16Clock_Number, uint16_t u16Presalcer)
{
    uint32_t u32rtc_sc;
        
    u32rtc_sc  = RTC->SC; 
    u32rtc_sc &= ~(RTC_SC_RTCLKS_MASK | RTC_SC_RTCPS_MASK);
    u32rtc_sc |= RTC_SC_RTCLKS(u16Clock_Number) | RTC_SC_RTCPS(u16Presalcer); 
    RTC->SC   = u32rtc_sc;
}

/*****************************************************************************//*!
*
* @brief ��ȡRTC�жϱ�־λ.
*        
* @param none 
*
* @return RTC�жϱ�־λ.
*
*****************************************************************************/
__STATIC_INLINE uint8_t RTC_GetFlags(void)
{
  uint8_t bflag;
  
  bflag = RTC->SC & RTC_SC_RTIF_MASK; 
  
  return bflag;  
}


/*****************************************************************************//*!
*
* @brief ���RTC�жϱ�־λ.
*        
* @param none 
*
* @return none
*
*****************************************************************************/
__STATIC_INLINE void RTC_ClrFlags(void)
{
    RTC->SC |= RTC_SC_RTIF_MASK; 
}


/******************************************************************************
******************************************************************************/


void RTC_Init(RTC_ConfigType *pConfig);
void RTC_SetCallback(RTC_CallbackType pfnCallback);
void RTC_DeInit(void);


#ifdef __cplusplus
}
#endif
#endif /* RTC_H_ */
