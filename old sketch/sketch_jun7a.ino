#include "lv_conf.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"

// Display and touch
TFT_eSPI tft = TFT_eSPI();

// LVGL display buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[480 * 10]; // 10 lines buffer
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

// Touch calibration - try factory defaults for ST7796
// Common factory calibration values for 3.5" ST7796 displays:
uint16_t calData[5] = { 300, 3700, 240, 3600, 2 };  // Try factory values first
// Alternative factory values to try:
// uint16_t calData[5] = { 275, 3600, 264, 3532, 2 };  // Previous working values
// uint16_t calData[5] = { 300, 3700, 240, 3600, 2 };  // Another common set

// Backlight control pin
#define TFT_BL 27

// Display flush callback
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// Global touch variables for debugging
uint16_t touchX, touchY;

// Touch read callback
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    bool touched = tft.getTouch(&touchX, &touchY);

    if (touched) {
        data->state = LV_INDEV_STATE_PRESSED;
        
        // Use raw coordinates directly without any mapping
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print("Touch: X=");
        Serial.print(touchX);
        Serial.print(", Y=");
        Serial.print(touchY);

        // Check if touch is in settings button area (top-right corner)
        if (touchX > 430 && touchX < 480 && touchY > 0 && touchY < 50) {
            Serial.print(" [SETTINGS BUTTON AREA]");
        }
        Serial.println();
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// Brightness slider event handler (needs Arduino functions)
void ui_event_brightness_slider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * slider = lv_event_get_target(e);
        int32_t brightness = lv_slider_get_value(slider);
        
        // Convert brightness (10-100) to PWM value (25-255)
        int32_t pwm_value = map(brightness, 10, 100, 25, 255);
        
        // Set PWM for backlight control
        analogWrite(TFT_BL, pwm_value);
        
        Serial.print("Brightness: ");
        Serial.print(brightness);
        Serial.print("%, PWM: ");
        Serial.println(pwm_value);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting ESP32 Display...");

    // Initialize backlight with PWM
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200); // Start with ~80% brightness (200/255)

    // Initialize display
    tft.begin();
    tft.setRotation(3); // Try different landscape orientation
    tft.fillScreen(TFT_BLACK);

    // Set touch calibration
    tft.setTouch(calData);
    
    Serial.println("Current calibration values:");
    for(int i = 0; i < 5; i++) {
        Serial.print("calData[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(calData[i]);
    }
    
    Serial.println("Touch calibration set.");
    Serial.println("Touch CS pin: 33");
    Serial.println("Testing touch detection...");
    
    // Test if touch is working
    uint16_t testX, testY;
    if (tft.getTouch(&testX, &testY)) {
        Serial.println("Touch controller detected!");
    } else {
        Serial.println("Touch controller NOT detected!");
    }

    // Initialize LVGL
    lv_init();

    // Initialize display buffer
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);

    // Initialize display driver
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize input device driver
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Initialize UI from SquareLine Studio
    ui_init();

    Serial.println("Setup complete!");
    Serial.println("Display: 480x320");
    Serial.println("Settings button should be in top-right corner (around X=450, Y=20)");
    Serial.println("Touch the settings gear button to test...");
}

void loop()
{
    lv_timer_handler(); // Handle LVGL tasks
    delay(10);
}