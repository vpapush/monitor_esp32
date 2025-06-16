#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Точная калибровка из рабочего скетча
uint16_t calData[5] = { 263, 3443, 256, 3643, 4 };

// Backlight pin
#define TFT_BL 27

// Touch coordinates
uint16_t touchX, touchY;
bool lastTouchState = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== SETUP START ===");

    // Initialize backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    Serial.println("Backlight ON");

    // Initialize display - точно как в рабочем коде
    tft.begin();
    tft.setRotation(0); // Как в рабочем коде
    Serial.println("TFT initialized with rotation 0");

    // Set calibration - точные значения из рабочего кода
    tft.setTouch(calData);
    Serial.println("Touch calibration set:");
    for(int i = 0; i < 5; i++) {
        Serial.print("calData[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(calData[i]);
    }

    // Draw interface - как в рабочем коде
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLUE);
    tft.drawString("RST", tft.width()-36, 0, 2);

    // Add test button for debugging
    tft.fillRect(tft.width()-100, 50, 80, 40, TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("TEST", tft.width()-80, 60, 2);

    Serial.println("=== SETUP COMPLETE ===");
    Serial.print("Display: ");
    Serial.print(tft.width());
    Serial.print("x");
    Serial.println(tft.height());
    Serial.println("Touch the test button or anywhere on screen...");
    Serial.print("Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
}

void loop() {
    // Точно такая же логика как в рабочем коде
    bool touched = tft.getTouch(&touchX, &touchY);

    if (touched) {
        if (!lastTouchState) {
            // Новое касание
            Serial.print("NEW TOUCH - X: ");
            Serial.print(touchX);
            Serial.print(", Y: ");
            Serial.print(touchY);

            // Check if touch is in reset area
            if (touchX > (tft.width()-36) && touchY < 16) {
                Serial.println(" [RST BUTTON]");
                tft.fillScreen(TFT_WHITE);
                tft.setTextColor(TFT_BLUE);
                tft.drawString("RST", tft.width()-36, 0, 2);
                tft.fillRect(tft.width()-100, 50, 80, 40, TFT_RED);
                tft.setTextColor(TFT_WHITE);
                tft.drawString("TEST", tft.width()-80, 60, 2);
            }
            // Check if touch is in test button area
            else if (touchX > (tft.width()-100) && touchX < tft.width()-20 && 
                     touchY > 50 && touchY < 90) {
                Serial.println(" [TEST BUTTON HIT!]");
                // Draw feedback
                tft.fillCircle(touchX, touchY, 10, TFT_GREEN);
            }
            else {
                Serial.println(" [SCREEN TOUCH]");
                // Draw small circle at touch point
                tft.fillCircle(touchX, touchY, 5, TFT_BLUE);
            }
        }
        lastTouchState = true;
    } else {
        if (lastTouchState) {
            Serial.println("Touch RELEASED");
        }
        lastTouchState = false;
    }

    // Small delay to prevent flooding
    delay(50);
}