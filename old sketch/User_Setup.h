
// ST7796 driver setup for ESP32 with 3.5" display based on manufacturer code
// This matches the manufacturer's LCD_Init code

#define ST7796_DRIVER

// ESP32 pin configuration based on your display documentation
#define TFT_MISO 12  // SPI bus read data signal
#define TFT_MOSI 13  // SPI bus write data signal  
#define TFT_SCLK 14  // SPI bus clock signal
#define TFT_CS   15  // LCD screen selection control signal
#define TFT_DC    2  // LCD command/data selection control signal
#define TFT_RST  -1  // Connected to EN (reset pin shared with ESP32)

// Touch controller pins - according to board pinout
#define TOUCH_CS 33  // Resistive touch screen chip selection control signal

// SPI frequency
#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Color depth
#define COLOUR_DEPTH 16

// Display size
#define TFT_WIDTH  480
#define TFT_HEIGHT 320

// Load additional fonts if needed
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH
#define LOAD_FONT6  // Font 6. Large 48 pixel high font, needs ~2666 bytes in FLASH
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel high font, needs ~2438 bytes in FLASH
#define LOAD_FONT8  // Font 8. Large 75 pixel high font needs ~3256 bytes in FLASH

#define SMOOTH_FONT

// Display orientation
#define TFT_ROTATION 1  // 0-3 for different orientations
