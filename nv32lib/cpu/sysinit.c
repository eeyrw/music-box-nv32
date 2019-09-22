/*****************************************************************************
* @brief provide system init routine/configuration for NV32Fxx.
*
*******************************************************************************/

#include "common.h"
#include "sysinit.h"
#include "sim.h"
#include "uart.h"
#include "ics.h"

/********************************************************************/

uint16_t global_pass_count = 0;
uint16_t global_fail_count = 0;


void print_sys_log(void);
void UART_InitPrint(void);

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: sysinit
*
* @brief  initalize system including SIM, ICS, UART, etc
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void sysinit (void)
{
    SIM_ConfigType  sSIMConfig = {{0},0};
    ICS_ConfigType  sICSConfig = {0};
    
    /* initialize the Pass/Fail counts to 0 */
     global_pass_count = 0;
     global_fail_count = 0; 
		
     EFMCR &=0xFFFF0001; // set wait state 1

#if defined(TRIM_IRC)    
    /* if not trimmed, do trim first */
    ICS_Trim(ICS_TRIM_VALUE);
#endif  
    /* 
     * Enable SWD pin, RESET pin 
     */
    /* 
     * NOTE: please make sure other register bits are also write-once and
     * need add other bit mask here if needed.
     */
#if defined(SPI0_PINREMAP)        
    sSIMConfig.u32PinSel |= SIM_PINSEL_SPI0PS_MASK;
#endif

#if defined(OUTPUT_BUSCLK)
    sSIMConfig.sBits.bEnableCLKOUT = 1;      /* output bus clock if enabled */
#endif
    
#if defined(DISABLE_NMI)
    sSIMConfig.sBits.bDisableNMI = 1;
#endif
    
    /* make sure clocks to peripheral modules are enabled */
    sSIMConfig.u32SCGC |= SIM_SCGC_SWD_MASK | SIM_SCGC_FLASH_MASK |
                         SIM_SCGC_UART0_MASK | SIM_SCGC_UART1_MASK | 
                         SIM_SCGC_UART2_MASK;
    
    sSIMConfig.sBits.bBusDiv |= BUSDIV_ENCODE;                           

		 
    SIM_Init(&sSIMConfig);                   /* initialize SIM */
        
#if defined(XOSC_STOP_ENABLE)   
    sICSConfig.oscConfig.bStopEnable = 1;    /*  enabled in stop mode */   
#endif          
    
#if defined(CRYST_HIGH_GAIN)    
    sICSConfig.oscConfig.bGain = 1;           /* high gain */      
#endif

    
#if  (EXT_CLK_FREQ_KHZ >=4000)
    sICSConfig.oscConfig.bRange = 1;           /* high range */
#endif
	
    sICSConfig.oscConfig.bEnable = 1;          /* enable OSC */
    sICSConfig.u32ClkFreq = EXT_CLK_FREQ_KHZ;
    
#if     defined(USE_FEE)    
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE;    
#elif	defined(USE_FBE_OSC)
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FBE_OSC;    
#elif	defined(USE_FEE_OSC)   
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE_OSC;    
#elif	defined(USE_FBILP)   
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FBILP;
#elif	defined(USE_FBELP)   
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FBELP; 		
#endif      

    ICS_Init(&sICSConfig);   /* initialize ICS */

    /* initialize UART for printing */
    //UART_InitPrint();                        

#if	defined(PRINT_SYS_LOG)
    //print_sys_log();
#endif
    
 }

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: print_sys_log
*
* @brief  print system reset sources
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void print_sys_log(void) 
{
    uint8_t u8Rst = 0;
    uint8_t u8FamID,u8SubFamID,u8RevID,u8PinID;
    uint32_t u32Status;
    
    u32Status = SIM_GetStatus(0xFF);        /* get all status bits */
    
    /* get all IDs */
    u8FamID = SIM_ReadID(ID_TYPE_FAMID);    
    u8SubFamID = SIM_ReadID(ID_TYPE_SUBFAMID);    
    u8RevID = SIM_ReadID(ID_TYPE_REVID);    
    u8PinID = SIM_ReadID(ID_TYPE_PINID);    
   
    printf("\n\n--System Log BEGINS--\n\n");
    printf("\n Familly ID = 0x%x, Sub-family ID = 0x%x, Revision ID = 0x%x, Pin ID = 0x%x \n",
            u8FamID, u8SubFamID, u8RevID, u8PinID);

    if((u32Status & SIM_SRSID_POR_MASK) && (u32Status & SIM_SRSID_LVD_MASK)) 
    {
      u8Rst = 1;
      printf(" Power On Reset\n");
    }
    
    if(!(u32Status & SIM_SRSID_POR_MASK) && (u32Status & SIM_SRSID_LVD_MASK)) 
    {
      u8Rst = 1;
      printf(" LVD Reset\n");
    }
    
    if(u32Status & SIM_SRSID_WDOG_MASK)                 
    {
      u8Rst = 1;
      printf(" WDOG Reset\n");
    }
    
    
    if(u32Status & SIM_SRSID_PIN_MASK)
    {
      u8Rst = 1;
      printf(" Pin Reset\n");
    }
    if(u32Status & SIM_SRSID_LOC_MASK)
    {
      u8Rst = 1;
      printf(" Loss of Clock Reset\n");
    }    
    if(u32Status & SIM_SRSID_SACKERR_MASK)
    {
      u8Rst = 1;
      printf(" Stop Mode Acknowledge Error Reset\n");
    }    
    if(u32Status & SIM_SRSID_MDMAP_MASK)
    {
      u8Rst = 1;
      printf(" MDM-AP System Reset Request\n");
    }    
    if(u32Status & SIM_SRSID_SW_MASK)
    {
      u8Rst = 1;
      printf(" Software/SYSRESETREQ Reset\n");
    }    
    if(u32Status & SIM_SRSID_LOCKUP_MASK)
    {
      u8Rst = 1;
      printf(" Core lockup Reset\n");
    }    

    if(u8Rst != 1)
    {
      printf("SWD Reset\n");
    }
    
    printf("\n--System Log ENDS--\n\n");
  }


/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: end_test
*
* @brief  print test result (pass/fail counts) after end of test
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void end_test(void)
{

  if(global_fail_count==0){
    printf("\n global_pass_count = 0x%02x%02x",(uint8)(global_pass_count>>8),(uint8)global_pass_count);
    printf("\n\n TEST PASSED");       
  } else{
    printf("\n global_pass_count = 0x%02x%02x",(uint8)(global_pass_count>>8),(uint8)global_pass_count);
    printf("\n global_fail_count = 0x%02x%02x",(uint8)(global_fail_count>>8),(uint8)global_fail_count);
    printf("\n\n TEST FAILED");
  }
  
  printf("\n\n TEST FINISHED");  
  
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: UART_InitPrint
*
* @brief initialize UART for print on port defined by TERM_PORT.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void UART_InitPrint(void)
{
    UART_ConfigType sConfig;

    sConfig.u32SysClkHz = BUS_CLK_HZ;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    SIM_RemapUART0Pin();
    
    UART_Init (TERM_PORT, &sConfig);
}
