
// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.3.0
// LVGL VERSION: 8.3.6
// PROJECT: SquareLine_Project

#include "ui.h"

void ui_event_Screen1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
}

void ui_event_settings_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen_Settings, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_Screen_Settings_screen_init);
    }
}

void ui_event_back_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, &ui_Screen1_screen_init);
    }
}

void ui_event_connect_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        // TODO: Implement WiFi connection and printer setup
        lv_label_set_text(ui_status_label, "Status: Connecting...");
    }
}

// Brightness slider handler moved to sketch_jun7a.ino
// This is declared as extern there
extern void ui_event_brightness_slider(lv_event_t * e);
