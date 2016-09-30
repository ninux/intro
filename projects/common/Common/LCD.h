/**
 * \file
 * \brief Interface to handle the LCD display
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This is the interface to the Nokia LCD Display.
 */

#ifndef SOURCES_LCD_H_
#define SOURCES_LCD_H_

#include "Platform.h"
#if PL_CONFIG_HAS_LCD

#if PL_CONFIG_HAS_RADIO
#include "RApp.h"
#include "RNWK.h"
#include "RPHY.h"

uint8_t LCD_HandleRemoteRxMessage(RAPP_MSG_Type type, uint8_t size, uint8_t *data, RNWK_ShortAddrType srcAddr, bool *handled, RPHY_PacketDesc *packet);
#endif

/*!
 * \brief Driver de-initialization
 */
void LCD_Deinit(void);

/*!
 * \brief Driver initialization
 */
void LCD_Init(void);

#endif /* PL_CONFIG_HAS_LCD */

#endif /* SOURCES_LCD_H_ */
