
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
    static unsigned long lastValidTouch = 0;
    static unsigned long lastCheckTime = 0;
    
    // Проверяем касание не чаще чем раз в 30мс (экономим ресурсы)
    if (millis() - lastCheckTime < 30) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }
    lastCheckTime = millis();
    
    bool currentTouchState = ts.touched();
    
    if (currentTouchState) {
        TS_Point p = ts.getPoint();
        
        // УСИЛЕННАЯ ФИЛЬТРАЦИЯ ЛОЖНЫХ СРАБАТЫВАНИЙ
        // 1. Проверяем на классические ложные координаты
        if (p.x == -4096 || p.y == -4096 || p.x < 0 || p.y < 0 || 
            p.x > 4095 || p.y > 4095) {
            data->state = LV_INDEV_STATE_RELEASED;
            return;
        }
        
        // 2. Проверяем давление - должно быть в разумных пределах
        if (p.z < 300 || p.z > 3000) {
            data->state = LV_INDEV_STATE_RELEASED;
            return;
        }
        
        // 3. Антидребезг - игнорируем касания слишком близко по времени
        if (millis() - lastValidTouch < 100) {
            data->state = LV_INDEV_STATE_RELEASED;
            return;
        }
        
        // 4. Проверяем валидный диапазон координат (с учетом калибровки)
        if (p.x < 250 || p.x > 3850 || p.y < 350 || p.y > 3650) {
            data->state = LV_INDEV_STATE_RELEASED;
            return;
        }
        
        // 5. Дополнительная проверка - координаты не должны быть на краю
        if ((p.x < 500 && p.y < 500) || (p.x > 3500 && p.y > 3500)) {
            data->state = LV_INDEV_STATE_RELEASED;
            return;
        }
        
        // Если дошли сюда - касание валидное
        lastValidTouch = millis();
        
        Serial.print("Valid touch: X=");
        Serial.print(p.x);
        Serial.print(", Y=");
        Serial.print(p.y);
        Serial.print(", Z=");
        Serial.print(p.z);
        
        // Калибровка на основе данных производителя (rotation 3)
        // Используем более точные значения калибровки
        if (p.y < 0) p.y = 0;
        if (p.x > 4095) p.x = 4095;
        if (p.y > 4095) p.y = 4095;
        
        // Калибровка на основе спецификации производителя для ST7796S
        // rotation 3: width=480, height=320
        touchX = map(p.x, 250, 3850, 0, 479);
        touchY = map(p.y, 350, 3650, 0, 319);
        
        // Constrain to display bounds
        touchX = constrain(touchX, 0, 479);
        touchY = constrain(touchY, 0, 319);
        
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print(" -> Mapped: X=");
        Serial.print(touchX);
        Serial.print(", Y=");
        Serial.print(touchY);

        // Check if touch is in test button area (top-right corner)
        if (touchX > 430 && touchX < 480 && touchY > 0 && touchY < 50) {
            Serial.print(" [TEST BUTTON AREA!]");
        }
        
        // Check if touch is anywhere in reasonable screen area
        if (touchX >= 0 && touchX <= 479 && touchY >= 0 && touchY <= 319) {
            Serial.print(" [VALID SCREEN AREA]");
        } else {
            Serial.print(" [OUT OF BOUNDS!]");
        }
        
        Serial.println();
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void setup()
{
    Serial.begin(115200);
    delay(2000); // Give time for Serial to initialize
    Serial.println("=== ESP32 Display Setup Started ===");
    Serial.flush();

    // Initialize backlight with PWM
    Serial.println("1. Initializing backlight...");
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200); // Start with ~80% brightness (200/255)
    Serial.println("   Backlight OK");
    Serial.flush();

    // Initialize display
    Serial.println("2. Initializing TFT display...");
    tft.begin();
    Serial.println("   TFT begin OK");
    tft.setRotation(3); // Same orientation as working sketch
    Serial.println("   TFT rotation set");
    tft.fillScreen(TFT_BLACK);
    Serial.println("   TFT screen cleared");
    Serial.flush();

    // Initialize touch controller
    Serial.println("3. Initializing XPT2046 touch controller...");
    
    // Дополнительная инициализация SPI для тачскрина
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    delay(100);
    
    ts.begin();
    Serial.println("   Touch begin OK");
    ts.setRotation(3); // Match display rotation
    Serial.println("   Touch rotation set");
    
    // Очистка буфера тачскрина от ложных срабатываний
    Serial.println("   Clearing touch buffer...");
    for(int i = 0; i < 10; i++) {
        if (ts.touched()) {
            ts.getPoint(); // Читаем и выбрасываем ложные данные
        }
        delay(10);
    }
    Serial.println("   Touch buffer cleared");
    Serial.flush();
    
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
    Serial.println("4. Initializing LVGL...");
    lv_init();
    Serial.println("   LVGL init OK");
    Serial.flush();

    // Initialize display buffer
    Serial.println("5. Setting up display buffer...");
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);
    Serial.println("   Display buffer OK");
    Serial.flush();

    // Initialize display driver
    Serial.println("6. Registering display driver...");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println("   Display driver OK");
    Serial.flush();

    // Initialize input device driver
    Serial.println("7. Registering input driver...");
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    Serial.println("   Input driver OK");
    Serial.flush();

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
    Serial.flush();

    Serial.println("=== SETUP COMPLETE ===");
    Serial.println("Display: 480x320");
    Serial.println("Settings button should be in top-right corner (around X=450, Y=20)");
    Serial.println("Touch the test button or anywhere on screen...");
    Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
    Serial.flush();
}

void loop()
{
    static unsigned long lastHeartbeat = 0;
    static unsigned long lastTouchCheck = 0;
    static int loopCounter = 0;
    
    // Handle LVGL tasks with error handling
    lv_timer_handler();
    
    // Убираем постоянную проверку тача - она вызывает спам в консоли
    // Тач будет проверяться только когда LVGL запрашивает данные
    
    // Heartbeat every 10 seconds to show we're alive (убираем статус тача - он ложно срабатывает)
    if (millis() - lastHeartbeat > 10000) {
        loopCounter++;
        Serial.println("Loop " + String(loopCounter) + " - Free heap: " + String(ESP.getFreeHeap()) + " bytes");
        Serial.flush();
        lastHeartbeat = millis();
    }
    
    delay(10);
}
