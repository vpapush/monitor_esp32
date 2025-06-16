
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Точная калибровка из рабочего скетча
uint16_t calData[5] = { 263, 3443, 256, 3643, 4 };

// Backlight pin
#define TFT_BL 27

// Touch coordinates - точно как в рабочем коде
uint16_t px0 = 0, px1 = 0, py0 = 0, py1 = 0;

// Colors definitions - как в рабочем коде
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void LCD_Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t Size, uint16_t colour)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 
  if(x1<Size||x2<Size||y1<Size||y2<Size)
  {
    return;  
  }
  delta_x=x2-x1; //计算坐标增量 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1; //设置单步方向 
  else if(delta_x==0)incx=0;//垂直线 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;//水平线 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//画线输出 
  {  
     tft.fillCircle(uRow, uCol, Size, colour);
    xerr+=delta_x ; 
    yerr+=delta_y ; 
    if(xerr>distance) 
    { 
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance) 
    { 
      yerr-=distance; 
      uCol+=incy; 
    } 
  }  
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== SETUP START ===");

    // Initialize backlight
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    Serial.println("Backlight ON");

    // Initialize display - точно как в рабочем коде
    tft.init();
    tft.setRotation(0); // Как в рабочем коде
    Serial.println("TFT initialized with rotation 0");

    // Set calibration - точные значения из рабочего кода
    tft.setTouch(calData);
    Serial.println("Touch calibration set");

    // Draw interface - точно как в рабочем коде
    tft.fillScreen(WHITE);
    tft.setTextColor(BLUE);
    tft.drawString("RST", tft.width()-36, 0, 2);

    Serial.println("=== SETUP COMPLETE ===");
    Serial.print("Display: ");
    Serial.print(tft.width());
    Serial.print("x");
    Serial.println(tft.height());
    Serial.println("Settings button should be in top-right corner (around X=450, Y=20)");
    Serial.println("Touch the test button or anywhere on screen...");
    Serial.print("Free heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
}

void loop() {
    // Точно такая же логика как в рабочем touch_pen.ino
    if(tft.getTouch(&px0, &py0, 600)) // ВАЖНО: третий параметр 600!
    {
        if((px0 < tft.width()) && (py0 < tft.height())) 
        {
            if(px1 == 0xFFFF)
            {
                px1 = px0;
                py1 = py0;
            }
            
            if(px0 > (tft.width()-36) && (py0 < 16))
            {
                // RST button pressed
                Serial.println("RST button pressed!");
                tft.fillScreen(WHITE);
                tft.setTextColor(BLUE);
                tft.drawString("RST", tft.width()-36, 0, 2);
            } 
            else
            {
                // Draw line from previous point to current point
                LCD_Draw_Line(px1, py1, px0, py0, 2, RED);
                Serial.print("Drawing line from (");
                Serial.print(px1);
                Serial.print(",");
                Serial.print(py1);
                Serial.print(") to (");
                Serial.print(px0);
                Serial.print(",");
                Serial.print(py0);
                Serial.println(")");
            }          
            px1 = px0;
            py1 = py0;
        }
    }
    else
    {
        // No touch detected - reset px1
        px1 = 0xFFFF;
    }
}
