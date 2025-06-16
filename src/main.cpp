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

// Callback для чтения координат с тачскрина
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    // Используем tft.getTouch(), как в вашем рабочем скетче
    bool touched = tft.getTouch(&touchX, &touchY);

    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting based on the working sketch...");

    // Инициализация LVGL
    lv_init();

    // Инициализация дисплея. Порядок важен.
    tft.begin();
    tft.setRotation(3); // Та же ориентация (3), что и в вашем скетче
    tft.setTouch(calData); // Применяем ваши калибровочные данные

    // Управление подсветкой
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Настройка драйвера дисплея LVGL
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 480 * 10);
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
    lv_indev_t *indev = lv_indev_drv_register(&indev_drv);

    // --- Ваш UI от SquareLine Studio ---
    // ui_init(); // Раскомментируйте эту строку, когда добавите в проект файлы ui.c и ui.h
    
    // --- Тестовые объекты для проверки ---
    // Если ui_init() закомментирован, создадим тестовую надпись и курсор, чтобы видеть результат
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "It MUST work now!");
    lv_obj_center(label);

    lv_obj_t *cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, LV_SYMBOL_EDIT);
    lv_indev_set_cursor(indev, cursor_obj);

    Serial.println("Setup complete. Touch the screen.");
}

void loop() {
    lv_timer_handler(); // Обработчик LVGL
    delay(5);

    // --- Блок для проверки Монитора порта ---
    static uint32_t last_check_time = 0;
    if (millis() - last_check_time > 1000) { // Выполнять каждую секунду
        last_check_time = millis();
        Serial.println("Loop is running, Serial Monitor is OK!");
    }
}