/**
 * \file
 * \brief Main application file
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This provides the main application entry point.
 */

#include "Platform.h"
#include "Application.h"
#include "Event.h"
#include "LED.h"
#include "WAIT1.h"
#include "CS1.h"
#include "Keys.h"
#include "KeyDebounce.h"
#include "KIN1.h"
#if PL_CONFIG_HAS_SHELL
  #include "CLS1.h"
  #include "Shell.h"
#endif
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
  #include "RTOS.h"
#endif
#if PL_CONFIG_HAS_QUADRATURE
  #include "Q4CLeft.h"
  #include "Q4CRight.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_BOARD_IS_ROBO_V2
  #include "PORT_PDD.h"
#endif

#if PL_CONFIG_HAS_EVENTS
void APP_EventHandler(EVNT_Handle event) {
  switch(event) {
  case EVNT_STARTUP:
    LED1_On(); /* just do something */
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_WELCOME);
#endif
    EVNT_SetEvent(EVNT_LED_OFF);
    WAIT1_Waitms(500);
    break;
  case EVNT_LED_OFF:
    LED1_Off();
    break;
  case EVNT_LED_HEARTBEAT:
    LED1_Neg();
    break;

#if PL_CONFIG_HAS_KEYS
  #if PL_CONFIG_NOF_KEYS>=1
  case EVNT_SW1_PRESSED:
    LED1_Neg();
    //CLS1_SendStr("SW1 pressed\r\n", CLS1_GetStdio()->stdOut);
    SHELL_SendString("SW1 pressed\r\n");
    #if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_BUTTON);
    #endif
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=2
  case EVNT_SW2_PRESSED:
    SHELL_SendString("SW2 pressed\r\n");
    LED1_Neg();
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=3
  case EVNT_SW3_PRESSED:
    SHELL_SendString("SW3 pressed\r\n");
    LED1_Neg();
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=4
  case EVNT_SW4_PRESSED:
    SHELL_SendString("SW4 pressed\r\n");
    LED1_Neg();
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=5
  case EVNT_SW5_PRESSED:
    SHELL_SendString("SW5 pressed\r\n");
    LED1_Neg();
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=6
  case EVNT_SW6_PRESSED:
    SHELL_SendString("SW6 pressed\r\n");
    LED1_Neg();
    break;
  #endif
  #if PL_CONFIG_NOF_KEYS>=7
  case EVNT_SW7_PRESSED:
    SHELL_SendString("SW7 pressed\r\n");
    LED1_Neg();
    break;
  #endif
#endif /* PL_CONFIG_HAS_KEYS */

    /* \todo extend handler as needed */
  default:
    break;
   } /* switch */
}
#endif /* PL_CONFIG_HAS_EVENTS */

static const KIN1_UID RoboIDs[] = {
		/* 0: L20, V2 */ {{0x00,0x03,0x00,0x00,0x67,0xCD,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
		/* 1: L21, V2 */ {{0x00,0x05,0x00,0x00,0x4E,0x45,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
		/* 2: L4, V1  */ {{0x00,0x0B,0xFF,0xFF,0x4E,0x45,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x24}}, /* revert right motor */
		/* 3: L23, V2 */ {{0x00,0x0A,0x00,0x00,0x67,0xCD,0xB8,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}}, /* revert left & right motor */
		/* 4: L11, V2 */ {{0x00,0x19,0x00,0x00,0x67,0xCD,0xB9,0x11,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}}, /* revert right encoder, possible damaged motor driver? */
};

static void APP_AdoptToHardware(void) {
  KIN1_UID id;
  uint8_t res;

  res = KIN1_UIDGet(&id);
  if (res!=ERR_OK) {
    for(;;); /* error */
  }
#if PL_CONFIG_HAS_MOTOR
  if (KIN1_UIDSame(&id, &RoboIDs[ROBO_STYGER])) { /* L4 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE);	/* invert direction */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE);	/* invert direction */
#define ROBO_ID_NUMBER ROBO_STYGER
  }
  if (KIN1_UIDSame(&id, &RoboIDs[ROBO_GROUP])) { /* L4 */
	  MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), FALSE);	/* do not invert direction */
	  MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), FALSE);	/* do not invert direction */
#define ROBO_ID_NUMBER ROBO_GROUP
  }
#endif
#if PL_CONFIG_HAS_QUADRATURE && PL_CONFIG_BOARD_IS_ROBO_V2
  /* pull-ups for Quadrature Encoder Pins */
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
#endif
}

#if 0
static volatile int testVar;
static void (*foo)(void) = 1;
static volatile int var, var2;
static int *p = 0;

static void Critical(void) {
  CS1_CriticalVariable()

  var /= var2;
  var = *p;
  *p = 5;
  CS1_EnterCritical();
  testVar++;
  foo();
  CS1_ExitCritical();
}
#endif

#include "CLS1.h"

void APP_Start(void) {
#if PL_CONFIG_HAS_RTOS
#if configUSE_TRACE_HOOKS
  PTRC1_uiTraceStart();
#endif
#endif
  PL_Init();
#if PL_CONFIG_HAS_EVENTS
  EVNT_SetEvent(EVNT_STARTUP);
#endif
#if PL_CONFIG_HAS_SHELL && CLS1_DEFAULT_SERIAL
  // CLS1_SendStr((uint8_t*)"Hello World!\r\n", CLS1_GetStdio()->stdOut);
#endif
  APP_AdoptToHardware();
#if PL_CONFIG_HAS_RTOS
  vTaskStartScheduler(); /* start the RTOS, create the IDLE task and run my tasks (if any) */
  /* does usually not return! */
#else
  //EVNT_SetEvent(EVNT_STARTUP);
  for(;;) {
#if PL_CONFIG_HAS_KEYS
  #if PL_CONFIG_HAS_DEBOUNCE
    KEYDBNC_Process();
  #else
    KEY_Scan();
  #endif
#endif
#if PL_CONFIG_HAS_EVENTS
    EVNT_HandleEvent(APP_EventHandler, TRUE);
#endif
   //LED1_On();
   //LED2_On();
   //LED3_On();
  // Critical();
    //WAIT1_Waitms(25); /* just wait for some arbitrary time .... */
    //LED1_Off();
    //LED2_Off();
    //WAIT1_Waitms(25); /* just wait for some arbitrary time .... */
    //CLS1_SendStr((uint8_t*)"hello world!\r\n", CLS1_GetStdio()->stdOut);
  }
#endif
}
