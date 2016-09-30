/*
 * LCDMenu.h
 *
 *  Created on: 09.09.2016
 *      Author: Erich Styger Local
 */

#ifndef SOURCES_LCDMENU_H_
#define SOURCES_LCDMENU_H_

#include "Platform.h"

#if PL_CONFIG_HAS_LCD_MENU
typedef enum {
  LCDMENU_EVENT_INIT,
  LCDMENU_EVENT_PRE_DRAW, /* called for handlers pre-drawing items */
  LCDMENU_EVENT_DRAW,
  LCDMENU_EVENT_GET_TEXT, /* get menu text, returned in data handler */
  LCDMENU_EVENT_UP,
  LCDMENU_EVENT_DOWN,
  LCDMENU_EVENT_LEFT,
  LCDMENU_EVENT_RIGHT,
  LCDMENU_EVENT_ENTER,
} LCDMenu_EventType;

#define LCDMENU_ID_NONE     0   /* special menu ID for 'no' id */
#define LCDMENU_GROUP_ROOT  0   /* special ID for root menu group */

typedef struct LCDMenu_MenuItem_{
  uint8_t id;  /* unique ID of menu item, starting with 1 */
  uint8_t group; /* menu group, starting with 0 (root), then increasing numbers */
  uint8_t pos; /* position of menu in level, starting with 0 (top position) */
  uint8_t lvlUpID;  /* menu item level up, 0 for 'none' */
  uint8_t lvlDownID; /* menu item level down, 0 for 'none' */
  char *menuText; /* text of menu item */
  bool (*handler)(const struct LCDMenu_MenuItem_ *item, LCDMenu_EventType event, void **dataP); /* callback for menu item */
} LCDMenu_MenuItem;

void LCDMenu_OnEvent(LCDMenu_EventType event);

void LCDMenu_InitMenu(const LCDMenu_MenuItem *menus, uint8_t nofMenuItems, uint8_t selectedID);

/*!
 * \brief Driver de-initialization
 */
void LCDMenu_Deinit(void);

/*!
 * \brief Driver initialization
 */
void LCDMenu_Init(void);

#endif /* PL_CONFIG_HAS_LCD_MENU */

#endif /* SOURCES_LCDMENU_H_ */

