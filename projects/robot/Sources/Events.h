/* ###################################################################
**     Filename    : Events.h
**     Project     : robot
**     Processor   : MK22FX512VLK12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-10-19, 09:53, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "LEDPin1.h"
#include "BitIoLdd1.h"
#include "LEDPin2.h"
#include "BitIoLdd2.h"
#include "SW1.h"
#include "BitIoLdd3.h"
#include "AS1.h"
#include "ASerialLdd3.h"
#include "CLS1.h"
#include "FRTOS1.h"
#include "RTT1.h"
#include "SYS1.h"
#include "LED_IR.h"
#include "LEDpin3.h"
#include "BitIoLdd5.h"
#include "RefCnt.h"
#include "IR1.h"
#include "BitIoLdd6.h"
#include "IR2.h"
#include "BitIoLdd7.h"
#include "IR3.h"
#include "BitIoLdd8.h"
#include "IR4.h"
#include "BitIoLdd9.h"
#include "IR5.h"
#include "BitIoLdd10.h"
#include "IR6.h"
#include "BitIoLdd11.h"
#include "Q4CLeft.h"
#include "C12.h"
#include "BitIoLdd16.h"
#include "C23.h"
#include "BitIoLdd17.h"
#include "Q4CRight.h"
#include "C13.h"
#include "BitIoLdd18.h"
#include "C25.h"
#include "BitIoLdd19.h"
#include "MOTTU.h"
#include "DIRL.h"
#include "BitIoLdd12.h"
#include "PWMR.h"
#include "PwmLdd2.h"
#include "DIRR.h"
#include "BitIoLdd13.h"
#include "PWML.h"
#include "PwmLdd3.h"
#include "QuadInt.h"
#include "TimerIntLdd2.h"
#include "TU_QuadInt.h"
#include "RNET1.h"
#include "RF1.h"
#include "CE1.h"
#include "BitIoLdd22.h"
#include "CSN1.h"
#include "BitIoLdd23.h"
#include "IRQ1.h"
#include "ExtIntLdd2.h"
#include "SM1.h"
#include "SMasterLdd2.h"
#include "USB1.h"
#include "CDC1.h"
#include "Tx1.h"
#include "Rx1.h"
#include "USB0.h"
#include "TMOUT1.h"
#include "UTIL1.h"
#include "KIN1.h"
#include "WAIT1.h"
#include "KSDK1.h"
#include "HF1.h"
#include "CS1.h"
#include "XF1.h"
#include "I2C1.h"
#include "SDA1.h"
#include "BitIoLdd14.h"
#include "SCL1.h"
#include "BitIoLdd15.h"
#include "WAIT2.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MK22FN1M0LQ12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


void FRTOS1_vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationStackOverflowHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationTickHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationIdleHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationMallocFailedHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, the RTOS will call this hook in case memory
**         allocation failed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  QuadInt_OnInterrupt (module Events)
**
**     Component   :  QuadInt [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void QuadInt_OnInterrupt(void);

void RF1_OnActivate(void);
/*
** ===================================================================
**     Event       :  RF1_OnActivate (module Events)
**
**     Component   :  RF1 [nRF24L01]
**     Description :
**         Event called before accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RF1_OnDeactivate(void);
/*
** ===================================================================
**     Event       :  RF1_OnDeactivate (module Events)
**
**     Component   :  RF1 [nRF24L01]
**     Description :
**         Event called before releasing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RNET1_OnRadioEvent(RNET1_RadioEvent event);
/*
** ===================================================================
**     Event       :  RNET1_OnRadioEvent (module Events)
**
**     Component   :  RNET1 [RNet]
**     Description :
**         Event created for various radio states, like timeout, ack
**         received, data sent, ...
**     Parameters  :
**         NAME            - DESCRIPTION
**         event           - 
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
