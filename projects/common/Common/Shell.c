/**
 * \file
 * \brief Shell and console interface implementation.
 * \author Erich Styger
 *
 * This module implements the front to the console/shell functionality.
 */

#include "Platform.h"
#if PL_CONFIG_HAS_SHELL
#include "Shell.h"
#include "CLS1.h"
#include "Application.h"
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
#endif
#if PL_CONFIG_HAS_BLUETOOTH
  #include "BT1.h"
#endif
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_SHELL_QUEUE
  #include "ShellQueue.h"
#endif
#if PL_CONFIG_HAS_REFLECTANCE
  #include "Reflectance.h"
#endif
#if PL_CONFIG_HAS_SEGGER_RTT
  #include "RTT1.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_HAS_MCP4728
  #include "MCP4728.h"
#endif
#if PL_CONFIG_HAS_QUADRATURE
  #include "Q4CLeft.h"
  #include "Q4CRight.h"
#endif
#if PL_CONFIG_HAS_QUAD_CALIBRATION
  #include "QuadCalib.h"
#endif
#if PL_CONFIG_HAS_MOTOR_TACHO
  #include "Tacho.h"
#endif
#if PL_CONFIG_HAS_ULTRASONIC
  #include "Ultrasonic.h"
#endif
#if PL_CONFIG_HAS_PID
  #include "PID.h"
#endif
#if PL_CONFIG_HAS_DRIVE
  #include "Drive.h"
#endif
#if PL_CONFIG_HAS_TURN
  #include "Turn.h"
#endif
#if PL_CONFIG_HAS_LINE_FOLLOW
  #include "LineFollow.h"
#endif
#if PL_CONFIG_HAS_RADIO
  #include "RApp.h"
  #include "RNet_App.h"
  #include "RNetConf.h"
#endif
#if RNET_CONFIG_REMOTE_STDIO
  #include "RStdIO.h"
#endif
#if PL_CONFIG_HAS_REMOTE
  #include "Remote.h"
#endif
#if PL_CONFIG_HAS_LINE_MAZE
  #include "Maze.h"
#endif
#if PL_CONFIG_HAS_USB_CDC
  #include "CDC1.h"
#endif
#include "KIN1.h"

#define SHELL_HANDLER_ARRAY   1
#define SHELL_CONFIG_HAS_SHELL_EXTRA_CDC   (0 && PL_CONFIG_HAS_USB_CDC)
#define SHELL_CONFIG_HAS_SHELL_EXTRA_RTT   (1 && PL_CONFIG_HAS_SEGGER_RTT)
#define SHELL_CONFIG_HAS_SHELL_EXTRA_BT    (0 && PL_CONFIG_HAS_BLUETOOTH)
#define SHELL_CONFIG_HAS_SHELL_EXTRA_UART  (0)


#if SHELL_HANDLER_ARRAY
typedef struct {
  CLS1_ConstStdIOType *stdio;
  unsigned char *buf;
  size_t bufSize;
} SHELL_IODesc;

#if CLS1_DEFAULT_SERIAL && (SHELL_CONFIG_HAS_SHELL_EXTRA_CDC || SHELL_CONFIG_HAS_SHELL_EXTRA_RTT)
  static void SHELL_SendChar(uint8_t ch) {
    /* everything sent to the standard I/O will be sent to additional channels */
    CLS1_SendChar(ch);
  #if SHELL_CONFIG_HAS_SHELL_EXTRA_CDC
    CDC1_SendChar(ch); /* copy on CDC */
  #endif
  #if SHELL_CONFIG_HAS_SHELL_EXTRA_RTT
    RTT1_SendChar(ch); /* copy on RTT */
  #endif
  }

  /* copy on other I/Os */
  CLS1_ConstStdIOType SHELL_stdio =
  {
    (CLS1_StdIO_In_FctType)CLS1_ReadChar, /* stdin */
    (CLS1_StdIO_OutErr_FctType)SHELL_SendChar, /* stdout */
    (CLS1_StdIO_OutErr_FctType)SHELL_SendChar, /* stderr */
    CLS1_KeyPressed /* if input is not empty */
  };

  CLS1_ConstStdIOType *SHELL_GetStdio(void) {
    return &SHELL_stdio;
  }
#else
  CLS1_ConstStdIOType *SHELL_GetStdio(void) {
    return CLS1_GetStdio();
  }
#endif

static const SHELL_IODesc ios[] =
{
#if CLS1_DEFAULT_SERIAL && (SHELL_CONFIG_HAS_SHELL_EXTRA_CDC || SHELL_CONFIG_HAS_SHELL_EXTRA_RTT)
    /* use special stdio to copy to other channels */
    {&SHELL_stdio, CLS1_DefaultShellBuffer, sizeof(CLS1_DefaultShellBuffer)},
#elif CLS1_DEFAULT_SERIAL /* default Shell communication channel */
    {&CLS1_stdio, CLS1_DefaultShellBuffer, sizeof(CLS1_DefaultShellBuffer)},
#endif
#if SHELL_CONFIG_HAS_SHELL_EXTRA_RTT
    {&RTT1_stdio, RTT1_DefaultShellBuffer, sizeof(RTT1_DefaultShellBuffer)},
#endif
#if SHELL_CONFIG_HAS_SHELL_EXTRA_UART
    {&AS1_stdio, AS1_DefaultShellBuffer, sizeof(AS1_DefaultShellBuffer)},
#endif
#if SHELL_CONFIG_HAS_SHELL_EXTRA_CDC
    {&CDC1_stdio, CDC1_DefaultShellBuffer, sizeof(CDC1_DefaultShellBuffer)},
#endif
#if SHELL_CONFIG_HAS_SHELL_EXTRA_BT
    {&BT1_stdio, BT1_DefaultShellBuffer, sizeof(BT1_DefaultShellBuffer)},
#endif
};

#endif

/* forward declaration */
static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand, /* Processor Expert Shell component, is first in list */
  SHELL_ParseCommand, /* our own module parser */
#if FRTOS1_PARSE_COMMAND_ENABLED
  FRTOS1_ParseCommand, /* FreeRTOS shell parser */
#endif
#if defined(BT1_PARSE_COMMAND_ENABLED) && BT1_PARSE_COMMAND_ENABLED
  BT1_ParseCommand,
#endif
#if PL_CONFIG_HAS_BUZZER
  BUZ_ParseCommand,
#endif
#if PL_CONFIG_HAS_REFLECTANCE
  #if REF_PARSE_COMMAND_ENABLED
  REF_ParseCommand,
  #endif
#endif
#if PL_CONFIG_HAS_MOTOR
  MOT_ParseCommand,
#endif
#if PL_CONFIG_HAS_MCP4728
   MCP4728_ParseCommand,
#endif
#if PL_CONFIG_HAS_QUADRATURE
  Q4CLeft_ParseCommand,
  Q4CRight_ParseCommand,
#endif
#if PL_CONFIG_HAS_QUAD_CALIBRATION
   QUADCALIB_ParseCommand,
#endif
#if PL_CONFIG_HAS_MOTOR_TACHO
  TACHO_ParseCommand,
#endif
#if PL_CONFIG_HAS_ULTRASONIC
  US_ParseCommand,
#endif
#if PL_CONFIG_HAS_PID
  PID_ParseCommand,
#endif
#if KIN1_PARSE_COMMAND_ENABLED
  KIN1_ParseCommand,
#endif
#if PL_CONFIG_HAS_DRIVE
  DRV_ParseCommand,
#endif
#if PL_CONFIG_HAS_TURN
  TURN_ParseCommand,
#endif
#if PL_CONFIG_HAS_LINE_FOLLOW
  LF_ParseCommand,
#endif
#if PL_CONFIG_HAS_RADIO
#if RNET1_PARSE_COMMAND_ENABLED
  RNET1_ParseCommand,
#endif
  RNETA_ParseCommand,
#endif
#if PL_CONFIG_HAS_REMOTE
  REMOTE_ParseCommand,
#endif
#if PL_CONFIG_HAS_LINE_MAZE
  MAZE_ParseCommand,
#endif
  NULL /* Sentinel */
};

static uint32_t SHELL_val; /* used as demo value for shell */

void SHELL_SendString(unsigned char *msg) {
#if PL_CONFIG_HAS_SHELL_QUEUE
  SQUEUE_SendString(msg);
#elif CLS1_DEFAULT_SERIAL
  CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
#else
#endif
}

/*!
 * \brief Prints the help text to the console
 * \param io StdIO handler
 * \return ERR_OK or failure code
 */
static uint8_t SHELL_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr("Shell", "Shell commands\r\n", io->stdOut);
  CLS1_SendHelpStr("  help|status", "Print help or status information\r\n", io->stdOut);
  CLS1_SendHelpStr("  val <num>", "Assign number value\r\n", io->stdOut);
  return ERR_OK;
}

/*!
 * \brief Prints the status text to the console
 * \param io StdIO handler
 * \return ERR_OK or failure code
 */
static uint8_t SHELL_PrintStatus(const CLS1_StdIOType *io) {
  uint8_t buf[16];

  CLS1_SendStatusStr("Shell", "\r\n", io->stdOut);
  UTIL1_Num32sToStr(buf, sizeof(buf), SHELL_val);
  UTIL1_strcat(buf, sizeof(buf), "\r\n");
  CLS1_SendStatusStr("  val", buf, io->stdOut);
  return ERR_OK;
}

static uint8_t SHELL_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  uint32_t val;
  const unsigned char *p;

  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "Shell help")==0) {
    *handled = TRUE;
    return SHELL_PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "Shell status")==0) {
    *handled = TRUE;
    return SHELL_PrintStatus(io);
  } else if (UTIL1_strncmp(cmd, "Shell val ", sizeof("Shell val ")-1)==0) {
    p = cmd+sizeof("Shell val ")-1;
    if (UTIL1_xatoi(&p, &val)==ERR_OK) {
      SHELL_val = val;
      *handled = TRUE;
    } else {
      return ERR_FAILED; /* wrong format of command? */
    }
  }
  return ERR_OK;
}

#if PL_CONFIG_HAS_RTOS
static void ShellTask(void *pvParameters) {
#if SHELL_HANDLER_ARRAY
  int i;
#endif
  /* \todo Extend as needed */

  (void)pvParameters; /* not used */
#if SHELL_HANDLER_ARRAY
  /* initialize buffers */
  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
    ios[i].buf[0] = '\0';
  }
#endif
#if CLS1_DEFAULT_SERIAL
  (void)CLS1_ParseWithCommandTable((unsigned char*)CLS1_CMD_HELP, ios[0].stdio, CmdParserTable);
#endif

  for(;;) {
#if SHELL_HANDLER_ARRAY
    /* process all I/Os */
    for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
      (void)CLS1_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
    }
#endif
#if PL_CONFIG_HAS_SHELL_QUEUE
#if PL_CONFIG_SQUEUE_SINGLE_CHAR
    {
        /*! \todo Handle shell queue */
    }
#else /* PL_CONFIG_SQUEUE_SINGLE_CHAR */
    {
      const unsigned char *msg;

      msg = SQUEUE_ReceiveMessage();
      if (msg!=NULL) {
        CLS1_SendStr(msg, CLS1_GetStdio()->stdOut);
        FRTOS1_vPortFree((void*)msg);
      }
    }
#endif /* PL_CONFIG_SQUEUE_SINGLE_CHAR */
#endif /* PL_CONFIG_HAS_SHELL_QUEUE */
    FRTOS1_vTaskDelay(10/portTICK_PERIOD_MS);
  } /* for */
}
#endif /* PL_CONFIG_HAS_RTOS */

void SHELL_Init(void) {
  SHELL_val = 0;
#if !CLS1_DEFAULT_SERIAL && PL_CONFIG_CONFIG_HAS_BLUETOOTH
  (void)CLS1_SetStdio(&BT_stdio); /* use the Bluetooth stdio as default */
#endif
#if PL_CONFIG_HAS_RTOS
  if (FRTOS1_xTaskCreate(ShellTask, "Shell", configMINIMAL_STACK_SIZE+100, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
#endif
}

void SHELL_Deinit(void) {
}

#endif /* PL_CONFIG_HAS_SHELL */
