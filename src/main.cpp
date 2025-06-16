
#include "lv_conf.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
// #include "ui.h" // Раскомментируйте, когда добавите файлы от SquareLine Studio

// Display and touch
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS);

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

// Touch read callback using XPT2046_Touchscreen
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        
        // Handle negative values (common issue with XPT2046)
        if (p.x < 0) p.x = 0;
        if (p.y < 0) p.y = 0;
        if (p.x > 4095) p.x = 4095;
        if (p.y > 4095) p.y = 4095;
        
        // Map coordinates for rotation 3 (landscape, USB on right)
        // These values are typical for 3.5" displays with XPT2046
        touchX = map(p.y, 300, 3700, 0, 479);  // Y maps to X for rotation 3
        touchY = map(p.x, 400, 3600, 319, 0);  // X maps to inverted Y for rotation 3
        
        // Constrain to display bounds
        touchX = constrain(touchX, 0, 479);
        touchY = constrain(touchY, 0, 319);
        
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print("Touch: Raw(");
        Serial.print(p.x);
        Serial.print(",");
        Serial.print(p.y);
        Serial.print(",");
        Serial.print(p.z);
        Serial.print(") -> Screen(");
        Serial.print(touchX);
        Serial.print(",");
        Serial.print(touchY);
        Serial.print(")");

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
    Serial.println("Starting ESP32 Display based on working sketch...");

    // Initialize backlight with PWM
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200); // Start with ~80% brightness (200/255)

    // Initialize display
    tft.begin();
    tft.setRotation(3); // Same orientation as working sketch
    tft.fillScreen(TFT_BLACK);

    // Initialize touch controller
    Serial.println("Initializing XPT2046 touch controller...");
    ts.begin();
    ts.setRotation(3); // Match display rotation
    
    Serial.println("Touch CS pin: 33");
    Serial.println("Testing XPT2046 touch detection...");
    
    // Test touch detection
    bool touchDetected = false;
    for(int i = 0; i < 5; i++) {
        if (ts.touched()) {
            TS_Point p = ts.getPoint();
            Serial.print("Touch detected on attempt ");
            Serial.print(i + 1);
            Serial.print(": Raw X=");
            Serial.print(p.x);
            Serial.print(", Raw Y=");
            Serial.print(p.y);
            Serial.print(", Pressure=");
            Serial.println(p.z);
            touchDetected = true;
            break;
        }
        delay(100);
    }
    
    if (!touchDetected) {
        Serial.println("XPT2046 touch controller NOT detected after 5 attempts!");
        Serial.println("Please touch the screen to test...");
    } else {
        Serial.println("XPT2046 touch controller detected successfully!");
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

    // Create test UI elements
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Touch screen test!\nTouch anywhere to see coordinates");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(label);
    
    // Test button in top-right corner (settings area)
    lv_obj_t *test_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(test_btn, 50, 40);
    lv_obj_align(test_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_t *btn_label = lv_label_create(test_btn);
    lv_label_set_text(btn_label, "TEST");
    lv_obj_center(btn_label);

    Serial.println("Setup complete!");
    Serial.println("Display: 480x320");
    Serial.println("Settings button should be in top-right corner (around X=450, Y=20)");
    Serial.println("Touch the test button or anywhere on screen...");
}

void loop()
{
    lv_timer_handler(); // Handle LVGL tasks
    delay(10);
}
