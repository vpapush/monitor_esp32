#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

void ui_event_Screen1(lv_event_t * e);
void ui_event_settings_btn(lv_event_t * e);
void ui_event_back_btn(lv_event_t * e);
void ui_event_connect_btn(lv_event_t * e);
extern void ui_event_brightness_slider(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif