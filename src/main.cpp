#include "lv_conf.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Display
TFT_eSPI tft = TFT_eSPI();

// LVGL display buffer
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[480 * 10]; // 10 lines buffer
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

// Touch calibration - точно те же значения из рабочего скетча
uint16_t calData[5] = { 300, 3700, 240, 3600, 2 };

// Backlight control pin
#define TFT_BL 27

// Global touch variables for debugging
uint16_t touchX, touchY;

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

// Touch read callback - точно как в рабочем коде
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

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== ESP32 Display Setup Started ===");

    // Initialize backlight with PWM
    Serial.println("1. Initializing backlight...");
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200); // Start with ~80% brightness (200/255)
    Serial.println("   Backlight OK");

    // Initialize display
    Serial.println("2. Initializing TFT display...");
    tft.begin();
    tft.setRotation(3); // Same orientation as working sketch
    tft.fillScreen(TFT_BLACK);
    Serial.println("   TFT initialized");

    // Set touch calibration - точно как в рабочем коде
    Serial.println("3. Setting touch calibration...");
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
    Serial.println("4. Initializing LVGL...");
    lv_init();
    Serial.println("   LVGL init OK");

    // Initialize display buffer
    Serial.println("5. Setting up display buffer...");
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);
    Serial.println("   Display buffer OK");

    // Initialize display driver
    Serial.println("6. Registering display driver...");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println("   Display driver OK");

    // Initialize input device driver
    Serial.println("7. Registering input driver...");
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    Serial.println("   Input driver OK");

    // Create test UI elements
    Serial.println("8. Creating UI elements...");
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Touch screen test!\nTouch anywhere to see coordinates");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(label);
    Serial.println("   Main label created");

    // Test button in top-right corner (settings area)
    lv_obj_t *test_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(test_btn, 50, 40);
    lv_obj_align(test_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_t *btn_label = lv_label_create(test_btn);
    lv_label_set_text(btn_label, "TEST");
    lv_obj_center(btn_label);
    Serial.println("   Test button created");

    Serial.println("=== SETUP COMPLETE ===");
    Serial.println("Display: 480x320");
    Serial.println("Settings button should be in top-right corner (around X=450, Y=20)");
    Serial.println("Touch the test button or anywhere on screen...");
    Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
}

void loop()
{
    lv_timer_handler(); // Handle LVGL tasks
    delay(10);
}