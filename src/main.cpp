
// Этот код основан на вашем рабочем скетче "sketch_jun7a.ino"
#include <lvgl.h>
#include <TFT_eSPI.h>
// #include "ui.h" // Раскомментируйте, когда добавите файлы от SquareLine Studio

// Глобальный объект для дисплея и тачскрина
TFT_eSPI tft = TFT_eSPI();

// Буферы и драйверы LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[480 * 10];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

// Калибровочные данные из вашего рабочего скетча
uint16_t calData[5] = { 300, 3700, 240, 3600, 2 };

// Пин подсветки
#define TFT_BL 27

// Глобальные переменные для отладки тача
uint16_t touchX, touchY;

// Callback для отправки буфера LVGL на дисплей
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    
    lv_disp_flush_ready(disp);
}

// Callback для чтения координат с тачскрина (точная копия из рабочего скетча)
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    bool touched = tft.getTouch(&touchX, &touchY);

    if (touched) {
        data->state = LV_INDEV_STATE_PRESSED;
        
        // Используем сырые координаты напрямую без маппинга
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print("Touch: X=");
        Serial.print(touchX);
        Serial.print(", Y=");
        Serial.print(touchY);

        // Проверяем попадание в область кнопки настроек (правый верхний угол)
        if (touchX > 430 && touchX < 480 && touchY > 0 && touchY < 50) {
            Serial.print(" [SETTINGS BUTTON AREA]");
        }
        Serial.println();
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP32 Display based on working sketch...");

    // Инициализация подсветки с PWM
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200); // Начальная яркость ~80% (200/255)

    // Инициализация дисплея
    tft.begin();
    tft.setRotation(3); // Та же ориентация, что и в рабочем скетче
    tft.fillScreen(TFT_BLACK);

    // Установка калибровки тача (ВАЖНО: после tft.begin())
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
    
    // Тест работы тача
    uint16_t testX, testY;
    if (tft.getTouch(&testX, &testY)) {
        Serial.println("Touch controller detected!");
    } else {
        Serial.println("Touch controller NOT detected!");
    }

    // Инициализация LVGL
    lv_init();

    // Настройка буфера дисплея
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);

    // Настройка драйвера дисплея LVGL
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Настройка драйвера тачскрина LVGL
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // --- Ваш UI от SquareLine Studio ---
    // ui_init(); // Раскомментируйте эту строку, когда добавите в проект файлы ui.c и ui.h
    
    // --- Тестовые объекты для проверки ---
    // Если ui_init() закомментирован, создадим тестовую надпись и кнопку
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Touch screen test!\nTouch anywhere to see coordinates");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(label);
    
    // Тестовая кнопка в правом верхнем углу (область настроек)
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

void loop() {
    lv_timer_handler(); // Обработка задач LVGL
    delay(10);
}
