/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

/**
 * \file
 * \brief Semaphore usage
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * Module using semaphores.
 */

#include "Platform.h" /* interface to the platform */
#if PL_CONFIG_HAS_SEMAPHORE
#include "FRTOS1.h"
#include "Sem.h"
#include "LED.h"

#define USE_SEMAPHORES 1

#if USE_SEMAPHORES
static void vSlaveTask(void *pvParameters) {
  xSemaphoreHandle sem = (xSemaphoreHandle)pvParameters;

  if (sem==NULL) {
    for(;;) {
      /* should not be NULL? */
    }
  }
  for(;;) {
    /*! \todo Implement functionality */
	  if (xSemaphoreTake(sem, 100/portTICK_PERIOD_MS) != pdTRUE) {
		  LED2_On();
		  vTaskDelay(250/portTICK_PERIOD_MS);
		  LED2_Off();
		  vTaskDelay(250/portTICK_PERIOD_MS);
	  } else {
		  LED1_Off();
		  LED2_On();
		  vTaskDelay(1000/portTICK_PERIOD_MS);
		  LED1_Off();
		  LED2_Off();
	  }
  }
}

static void vMasterTask(void *pvParameters) {
  /*! \todo Understand functionality */
  xSemaphoreHandle sem = NULL;

  (void)pvParameters; /* parameter not used */
  sem = xSemaphoreCreateBinary();
  if (sem==NULL) { /* semaphore creation failed */
    for(;;){} /* error */
  }
  vQueueAddToRegistry(sem, "IPC_Sem");
  /* create slave task */
  if (xTaskCreate(vSlaveTask, "Slave", configMINIMAL_STACK_SIZE, sem, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
  for(;;) {
    if (xSemaphoreGive(sem) != pdTRUE) {
    	LED1_On();
    }/* give control to other task */
    LED1_On();
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}
#endif /* USE_SEMAPHORES */

void SEM_Deinit(void) {
}

/*! \brief Initializes module */
void SEM_Init(void) {
#if USE_SEMAPHORES
  if (xTaskCreate(vMasterTask, "Master", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
    for(;;){} /* error */
  }
#endif
}
#endif /* PL_CONFIG_HAS_SEMAPHORE */
