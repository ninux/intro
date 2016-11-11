/**
 * \file
 * \brief LED driver
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This is a generic module to drive one or multiple LEDs.
 */

#include "Platform.h"
#if PL_CONFIG_HAS_LEDS
#include "LED.h"

static uint8_t LED_val = 0;

void LED_Deinit(void) {
  LED1_Off();
  LED2_Off();
  LED3_Off();
#if PL_CONFIG_NOF_LEDS>=4
  #error "only 3 LEDs supported"
#endif

#if PL_CONFIG_HAS_RTOS
  // FRTOS1_vTaskDelete()
#endif
}

void LED_Init(void) {
  LED1_Off();
  LED2_Off();
  LED3_Off();

#if PL_CONFIG_HAS_RTOS
  // testing semaphores
  if (FRTOS1_xTaskCreate(LED_Task_Off, "LED_MasterTask", configMINIMAL_STACK_SIZE+100, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
	  for(;;){} /* error */
  }

  if (FRTOS1_xTaskCreate(LED_Task_On, "LED_SlaveTask", configMINIMAL_STACK_SIZE+100, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
	  for(;;){} /* error */
  }
#endif
}

// for testing semaphores
static void LED_Task_Off(void *pvParameters) {
	while (true) {
		LED1_Off();
		FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
		LED1_On();
		FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

// for testing semaphores
static void LED_Task_On(void *pvParameters) {
	while (true) {
		LED2_On();
		FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
		LED2_Off();
		FRTOS1_vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

uint8_t LED_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
	uint32_t val;
	const unsigned char *p;

	if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "LED help")==0) {
		*handled = TRUE;
		return LED_PrintHelp(io);
	} else if (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "LED status")==0) {
		*handled = TRUE;
		return LED_PrintStatus(io);
	} else if (UTIL1_strncmp(cmd, "LED val ", sizeof("LED val ")-1)==0) {
		p = cmd+sizeof("Shell val ")-1;
		if (UTIL1_xatoi(&p, &val)==ERR_OK) {
			LED_val = val;
			*handled = TRUE;
		} else {
			return ERR_FAILED; /* wrong format of command? */
		}
	} else if (UTIL1_strcmp((char*)cmd, "LED color")==0) {
		*handled = TRUE;
		return LED_PrintColor(io);
	}
	  return ERR_OK;
}

uint8_t LED_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr("LED", "LED commands\r\n", io->stdOut);
  CLS1_SendHelpStr("  help|status", "Print help or status information\r\n", io->stdOut);
  CLS1_SendHelpStr("  val <num>", "Assign number value\r\n", io->stdOut);
  return ERR_OK;
}

uint8_t LED_PrintStatus(const CLS1_StdIOType *io) {
  uint8_t buf[16];

  CLS1_SendStatusStr("LED", "\r\n", io->stdOut);
  UTIL1_Num32sToStr(buf, sizeof(buf), LED_val);
  UTIL1_strcat(buf, sizeof(buf), "\r\n");
  CLS1_SendStatusStr("  val", buf, io->stdOut);
  return ERR_OK;
}

uint8_t LED_PrintColor(const CLS1_StdIOType *io) {
	CLS1_SendHelpStr("  color", "red\r\n", io->stdOut);
	return ERR_OK;
}

#endif /* PL_CONFIG_HAS_LEDS */
