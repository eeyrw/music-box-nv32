/********************************************************************************
 *
 * ʵ�����ƣ�Ӳ������ADCת��(FIFOģʽ)
 * ʵ��ƽ̨��NV32������
 * ����оƬ��NV32F100FL64E
 * ʵ��Ч�����趨FIFO��ȣ���ʼ��ADCģ��,ADC��ʼ�ɼ����ݷ���FIFO�У�FIFO��
						��ȴ�Ӳ�������źš�
						1:����ת��ģʽ��  ��ADC��⵽��һ��Ӳ�������źź󣬿�ʼת��FIFO
							                �е�һ�����ݣ���⵽�ڶ���Ӳ�������źź󣬿�ʼ
															ת��FIFO�е�2�����ݣ��Դ�����ֱ��FIFO�е�����ȫ��
															ת���꣬Ȼ�󴥷�ADC�ж϶�ȡת���������ת����ɺ�
															��������µĴ������壬�������µ�ת�����ϡ�
															������FIFO����趨Ϊ3ʱ��ADCҪ��⵽����Ӳ�������ź�
															���ܽ�FIFO�е�����ȫ��ת����ɣ�Ȼ������жϡ�
						2:����ת��ģʽ,   ������Ӳ�������ź�ʱ���ᴥ��ADC����������������ʹ��
						                  FIFO����ģʽʱ�����жϷ������ж�ȡFIFO���ǰ����ADC
															Ӳ������ʹ�ܣ��ڶ�ȡ��FIFO���֮���ٴ�Ӳ������ʹ�ܣ�
															�������ݴ��ҡ�
***********************************************************************************/
#include "common.h"
#include "ics.h"
#include "sim.h"
#include "rtc.h"
#include "pit.h"
#include "uart.h"
#include "adc.h"
#include "etm.h"
#include "sysinit.h"

uint16_t u16ADC_ConversionBuff[16];
uint16_t u16ADC_ConversionCount = 0;
volatile uint8_t  u8ADC_ConversionFlag = 0;

int main (void);
void ADC_CallBack( void );
/********************************************************************/
int main (void)
{
    uint16_t u16ModuloValue;
    uint32_t        u32LoadValue0;
    ADC_ConfigType  sADC_Config = {0};
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;
    PIT_ConfigType  sPITConfig0;
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0;
  
    /*ϵͳ��ʼ��*/
    sysinit();
		
		
    printf("\nRunning the ADC_HWT_demo project.\r\n");
    /*********************ADCӲ������Դѡ��**********************************/
		/**************************************************************************
		 *ע��ETM2ͨ��4��NMI�ж��������Ź���PB4,��ѡ��ETM2ͨ��4��ΪADCӲ������Դʱ��
		 *    ��Ҫ����NMI�ж���������
     ****************************************************************************/
    SIM_TriggerADCByRTC();         //ѡ��RTC�������ADCת��
    //SIM_TriggerADCByPIT();         //ѡ��PIT�������ADC
    //SIM_TriggerADCByETM2Init();    //ѡ��ETM2��ʼ����ʱһ��ʱ�䴥��ADCת��
    //SIM_TriggerADCByETM2Match();   //ѡ��ETM2ƥ�����ʱһ��ʱ�䴥��ADCת��
		
    /**********����ETM2��ƥ��/��ʼ��������ADCת������ʱʱ��*****************/
    SIM_SetClockOutputDivide(0x07); //����ʱ�������Ƶ
    SIM_DelayETM2Trig2ADC(0xff);  //��ʱ����ģ��ֵ
		
    UART_WaitTxComplete(TERM_PORT);
    LED0_Init();
		
    /* ��ʼ��ADCģ��*/
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;       /*!< ADCʱ��Դ��Ƶϵ��Ϊ8 */
    sADC_Config.u8ClockSource = CLOCK_SOURCE_ADACK;   /*!< ADCʱ��Դѡ���첽ʱ�� */
    sADC_Config.u8Mode = ADC_MODE_12BIT;              /*!< 12λת�� */
    sADC_Config.sSetting.bIntEn = 1;                  /*!< ʹ���ж�*/
    sADC_Config.u8FiFoLevel = ADC_FIFO_LEVEL3;        /*!< 3��FIFO */
    sADC_Config.sSetting.bContinuousEn=0;             /*!< ����ת��ģʽ */
    sADC_Config.sSetting.bHardwareTriggerEn=1;        /*!< ʹ��Ӳ������ģʽ */
    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);    //��ʼ��ADCģ��
		
    /*RTC�������ADC */
	
    u16ModuloValue = 0x09;
    pRTC_Config->u16ModuloValue = u16ModuloValue; //�趨ģ��ֵ
    pRTC_Config->bInterruptEn=0;   //�����ж�
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;  //ѡ��1KHzʱ��Դ
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;   //ʱ�ӷ�Ƶϵ��100
    RTC_Init(pRTC_Config);
		
		
    /*PIT0�������ADC */
    /*
      u32LoadValue0   = 0xFFFFFF;       //�趨װ��ֵ
      pPIT_Config0->u32LoadValue      = u32LoadValue0;
      pPIT_Config0->bFreeze           = FALSE;    //�ڵ���ģʽʱ��Ȼ����
      pPIT_Config0->bModuleDis        = FALSE;    //ʹ��PITģ��
      pPIT_Config0->bChainMode        = FALSE;    //������ģʽ
      pPIT_Config0->bInterruptEn      = FALSE;    //�����ж�
      pPIT_Config0->bETMerEn          = TRUE;     //ʹ��PITͨ��
      PIT_Init(PIT_CHANNEL0, pPIT_Config0);
    */
    /*ѡ��ETM2ƥ�䴥��*/
    /*
     ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE);
     ETM_SetETMEnhanced(ETM2);
     ETM2->EXTTRIG |= ETM_EXTTRIG_CH1TRIG_MASK;//ʹ��ͨ��1ƥ�䴥��
     ETM_SetModValue(ETM2, 29999);  // ����ETM2ģֵ
     ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 5000); //����ETM2ͨ��ֵ
     ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV128);  //����ETM2ģ��ʱ��Դ����Ƶϵ��.
     NVIC_DisableIRQ(ETM2_IRQn);
    */
    /*ѡ��ETM2��ʼ����*/
    /*
    ETM_PWMInit(ETM2, ETM_PWMMODE_EDGEALLIGNED, ETM_PWM_HIGHTRUEPULSE);
    ETM_SetETMEnhanced(ETM2);
    ETM_SetCounterInitValue(ETM2, 3000);
    ETM2->EXTTRIG |= ETM_EXTTRIG_INITTRIGEN_MASK; //��ʼ������ʹ��
    ETM_SetModValue(ETM2, 9999); // ����ETM2ģֵ
    ETM_SetChannelValue(ETM2, ETM_CHANNEL_CHANNEL1, 5000);  //����ETM2ͨ��ֵ
    ETM_ClockSet(ETM2, ETM_CLOCK_SYSTEMCLOCK, ETM_CLOCK_PS_DIV128);  //����ETM2ģ��ʱ��Դ����Ƶϵ��.
     */
		 
    /***********����ת��ͨ�����ȴ������ź� ***********************/
    ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
    ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
    ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
    while(1)
    {
    }
}
/***************************************************************************
+FUNCTION----------------------------------------------------------------
*
* @brief  ADC�ص������У��ڻص��������ȡת�����
*
* @param  none
*
* @return none
*
*****************************************************************************/
void ADC_CallBack( void )
{
    uint8_t         u8Ch;
    LED0_Toggle();
    while( !ADC_IsFIFOEmptyFlag(ADC) )  ////���FIFO������Ч��������
    {
        if( u16ADC_ConversionCount < 3 )  //��ȡת������������FIFO�е�����ȫ������
        {
            u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
        }
        else
        {
            ADC_ReadResultReg(ADC);
        }
    }
    /*��ӡת�����*/
    printf("ADC conversion result as below:\r\n");
    for( u8Ch =0 ; u8Ch< u16ADC_ConversionCount; u8Ch ++)
    {
        printf("0x%x,",u16ADC_ConversionBuff[u8Ch]);
    }
    printf("\r\n");
    u16ADC_ConversionCount = 0;;
}
/********************************************************************/
