#include "Adafruit_SSD1306.h"
#include "dashboardi2c.h"

/*
 * Modified: I2C Oled Monochrome SSD1306 128x32
 * http:// 
 *
 * orinal: dashboard.cpp - Library to drive a 80x160px color LCD as rc truck dashboard
 *
 * https://github.com/Gamadril/ESP32_LASE
 * MIT License
 */


DashboardI2c::DashboardI2c() {}

void DashboardI2c::init(uint8_t value)
{

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  } else Serial.println(F("SSD1306 started OK"));

  display.clearDisplay();
  

  display.fillScreen(SSD1306_BLACK);
  display.setTextSize(1); // Draw scale text
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Powered by"); //, 120, 34, 1);
  display.setCursor(0, 12);
  display.print("Infotronikblog"); //, 120, 46, 1);
  display.display();
  delay(1500);
  display.clearDisplay();

  drawFrame();
    
  setSpeed(0);
  setRPM(0);
  setFuelLevel(0);
  setAdBlueLevel(0);
  setLeftIndicator(false);
  setRightIndicator(false);
  setLowBeamIndicator(false);
  setHighBeamIndicator(false);
  setFogLightIndicator(false);
    
}

// OK
void DashboardI2c::setLeftIndicator(boolean on)
{
    drawLeftIndicator(on ? SSD1306_WHITE : SSD1306_BLACK);
}

// OK
void DashboardI2c::setRightIndicator(boolean on)
{
    drawRightIndicator(on ? SSD1306_WHITE : SSD1306_BLACK);
}

// OK
void DashboardI2c::setSpeed(uint16_t value) { drawSpeed(value); }

// OK
void DashboardI2c::setRPM(uint16_t value) { drawRPM(value); }

// OK
void DashboardI2c::setFuelLevel(uint16_t value) { drawFuel(value); }

// OK
void DashboardI2c::setAdBlueLevel(uint16_t value) { drawAdBlue(value); }

// OK
void DashboardI2c::setLowBeamIndicator(boolean on)
{
    drawLowBeamIndicator(on ? SSD1306_WHITE : SSD1306_BLACK);
}

void DashboardI2c::setHighBeamIndicator(boolean on)
{
  drawHighBeamIndicator(on ? SSD1306_WHITE : SSD1306_BLACK);
}

void DashboardI2c::setFogLightIndicator(boolean on)
{
    drawFogLightIndicator(on ? SSD1306_WHITE : SSD1306_BLACK);
}

void DashboardI2c::setGear(int8_t value)
{
    char symbol = ' ';
    if (value == -1)
    {
        symbol = 'R';
    }
    else if (value == 0)
    {
        symbol = 'N';
    }
    else if (value > 0 && value < 10)
    {
        symbol = '0' + value;
    }
    drawGear(symbol);
}

void DashboardI2c::setVolt(float value, float threshold)
{
    drawVolt(value, threshold);
}

void DashboardI2c::drawFrame()
{
    // speed
    drawArc(SPEED_CIRCLE_CENTER_X, SPEED_CIRCLE_CENTER_Y, SPEED_CIRCLE_RADIUS,
            SPEED_CIRCLE_ANGLE_START, SPEED_CIRCLE_ANGLE_END, SSD1306_WHITE);

    for (int16_t i = 0; i <= SPEED_CIRCLE_SEGMENTS; i++)
    {
        drawSep(SPEED_CIRCLE_CENTER_X, SPEED_CIRCLE_CENTER_Y, 3,
                SPEED_CIRCLE_ANGLE_START + i * SPEED_CIRCLE_SEGMENT_ANGLE, SPEED_CIRCLE_RADIUS,
                SSD1306_WHITE);
    }            

     // rpm
    drawArc(RPM_CIRCLE_CENTER_X, RPM_CIRCLE_CENTER_Y, RPM_CIRCLE_RADIUS, RPM_CIRCLE_ANGLE_START,
            RPM_CIRCLE_ANGLE_END, SSD1306_WHITE);
    drawArc(RPM_CIRCLE_CENTER_X, RPM_CIRCLE_CENTER_Y, RPM_CIRCLE_RADIUS - 1, RPM_CIRCLE_ANGLE_START,
            RPM_CIRCLE_ANGLE_END, SSD1306_WHITE);
    for (int16_t i = 0; i <= 5; i++)
    {
    drawSep(RPM_CIRCLE_CENTER_X, RPM_CIRCLE_CENTER_Y, 3, RPM_CIRCLE_ANGLE_START + i * 45,
            RPM_CIRCLE_RADIUS, SSD1306_WHITE);
    }

    // fuel
    drawArc(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, FUEL_CIRCLE_RADIUS, FUEL_CIRCLE_ANGLE_START,
            FUEL_CIRCLE_ANGLE_END, SSD1306_WHITE);
    for (int16_t i = 0; i <= 4; i++)
    {
        drawSep(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, 3,
                FUEL_CIRCLE_ANGLE_START + i * FUEL_CIRCLE_SEGMENT_ANGLE * 4, FUEL_CIRCLE_RADIUS,
                SSD1306_WHITE);
    }

   
    drawSep(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, 3, FUEL_CIRCLE_ANGLE_START,
            FUEL_CIRCLE_RADIUS, SSD1306_WHITE);
    drawSep(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, 3, FUEL_CIRCLE_ANGLE_START + 7 * 2,
            FUEL_CIRCLE_RADIUS, SSD1306_WHITE);
    drawSep(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, 3, FUEL_CIRCLE_ANGLE_START + 7 * 4,
            FUEL_CIRCLE_RADIUS, SSD1306_WHITE);

    drawPump(FUEL_CIRCLE_CENTER_X + 11, FUEL_CIRCLE_CENTER_Y - 4, SSD1306_WHITE);

    // adblue
    drawArc(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, ADBLUE_CIRCLE_RADIUS,
            ADBLUE_CIRCLE_ANGLE_START, ADBLUE_CIRCLE_ANGLE_END, SSD1306_WHITE);
    for (int16_t i = 0; i <= 4; i++)
    {
        drawSep(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, 3,
                ADBLUE_CIRCLE_ANGLE_START + i * ADBLUE_CIRCLE_SEGMENT_ANGLE * 4, ADBLUE_CIRCLE_RADIUS,
                SSD1306_WHITE);
    }
    drawSep(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, 3, ADBLUE_CIRCLE_ANGLE_START,
            ADBLUE_CIRCLE_RADIUS, SSD1306_WHITE);
    drawSep(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, 3, ADBLUE_CIRCLE_ANGLE_START + 7 * 2,
            ADBLUE_CIRCLE_RADIUS, SSD1306_WHITE);
    drawSep(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, 3, ADBLUE_CIRCLE_ANGLE_START + 7 * 4,
            ADBLUE_CIRCLE_RADIUS, SSD1306_WHITE);

    
    drawPump(ADBLUE_CIRCLE_CENTER_X - 17 , ADBLUE_CIRCLE_CENTER_Y - 4, SSD1306_WHITE);

    // gear
    drawGear('N');
  
    display.display();
    delay(100);
}

void DashboardI2c::drawArc(int16_t cx, int16_t cy, int16_t r, int16_t deg_start, int16_t deg_end,
                        uint16_t color)
{
    float sx, sy;
    uint16_t x, y;

    for (int16_t i = deg_start; i <= deg_end; i++)
    {  
        sx = cos(i * DEG2RAD);
        sy = sin(i * DEG2RAD);
        x = round(sx * r) + cx;
        y = round(sy * r) + cy;
        display.drawPixel(x, y, color);
    }
    display.display();
}

void DashboardI2c::drawSep(int16_t cx, int16_t cy, uint16_t len, int16_t deg_pos, uint16_t r,
                        uint16_t color)
{
    float sx, sy;
    uint16_t x0, y0, x1, y1;

    for (int8_t i = -1; i <= 1; i++)
    {
        sx = cos((i + deg_pos) * DEG2RAD);
        sy = sin((i + deg_pos) * DEG2RAD);
        x0 = round(sx * r) + cx;
        y0 = round(sy * r) + cy;
        x1 = round(sx * (r - len)) + cx;
        y1 = round(sy * (r - len)) + cy;
        display.drawLine(x0, y0, x1, y1, color);
    }
    display.display();
}

void DashboardI2c::drawNeedle(int16_t cx, int16_t cy, uint16_t len, int16_t deg_pos,
                           uint16_t radius_holder, uint16_t color_needle)
{
    float sx, sy;
    uint16_t x, y, xx, yy;

    display.fillCircle(cx, cy, radius_holder, SSD1306_WHITE);
    sx = cos(deg_pos * DEG2RAD);
    sy = sin(deg_pos * DEG2RAD);
    x = round(sx * len) + cx;
    y = round(sy * len) + cy;
    xx = -round(sx * 2) + cx;
    yy = -round(sy * 2) + cy;
    display.drawLine(x, y, xx, yy, color_needle);
    display.display();
}

void DashboardI2c::drawLeftIndicator(uint16_t color)
    {
      static uint16_t lastVal = SSD1306_BLACK;
    if (color == lastVal)
    {
        return;
    }
    lastVal = color;

    uint16_t x = INDICATOR_LEFT_CENTER_X;
    uint16_t y = INDICATOR_LEFT_CENTER_Y;

    uint8_t cols = sizeof(LEFT_INDICATOR_ICON_I2C[0]);
    uint8_t rows = sizeof(LEFT_INDICATOR_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (LEFT_INDICATOR_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, color);
            }
        }
    }
    display.display();
}

void DashboardI2c::drawRightIndicator(uint16_t color)
  {
    static uint16_t lastVal = SSD1306_BLACK;
    
    if (color == lastVal)
    {
        return;
    }
    lastVal = color;
    
    uint16_t x = INDICATOR_RIGHT_CENTER_X;
    uint16_t y = INDICATOR_RIGHT_CENTER_Y;

    uint8_t cols = sizeof(RIGHT_INDICATOR_ICON_I2C[0]);
    uint8_t rows = sizeof(RIGHT_INDICATOR_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (RIGHT_INDICATOR_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, color);
            }
        }
    }
    display.display();

}

void DashboardI2c::drawLowBeamIndicator(uint16_t color)
{
    static uint16_t lastVal = SSD1306_BLACK;
    if (color == lastVal)
    {
        return;
    }
    lastVal = color;

    uint16_t x = SCREEN_WIDTH / 2 + 10;
    uint16_t y = SCREEN_HEIGHT - 7;

    uint8_t cols = sizeof(LOW_BEAM_ICON_I2C[0]);
    uint8_t rows = sizeof(LOW_BEAM_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (LOW_BEAM_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, color);
            }
        }
    }
    display.display();
}

void DashboardI2c::drawHighBeamIndicator(uint16_t color)
{
    static uint16_t lastVal = SSD1306_BLACK;
    if (color == lastVal)
    {
        return;
    }
    lastVal = color;

    uint16_t x = SCREEN_WIDTH / 2 - 7;
    uint16_t y = SCREEN_HEIGHT - 7;

    uint8_t cols = sizeof(HIGH_BEAM_ICON_I2C[0]);
    uint8_t rows = sizeof(HIGH_BEAM_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (HIGH_BEAM_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, color);
            }
        }
    }
    display.display();
}

void DashboardI2c::drawFogLightIndicator(uint16_t color)
{
    static uint16_t lastVal = SSD1306_BLACK;
    if (color == lastVal)
    {
        return;
    }
    lastVal = color;

    uint16_t x = SCREEN_WIDTH / 2 - 24;
    uint16_t y = SCREEN_HEIGHT - 7;

    uint8_t cols = sizeof(FOG_LIGHT_ICON_I2C[0]);
    uint8_t rows = sizeof(FOG_LIGHT_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (FOG_LIGHT_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, color);
            }
        }
    }
    display.display();
}

void DashboardI2c::drawSpeed(uint16_t value)
{
    static uint16_t lastVal = 0xFFFF;
    if (value == lastVal)
    {
        return;
    }

    int16_t last_deg = map(lastVal, SPEED_MIN, SPEED_MAX, SPEED_CIRCLE_ANGLE_START, SPEED_CIRCLE_ANGLE_END);
    int16_t deg = map(value, SPEED_MIN, SPEED_MAX, SPEED_CIRCLE_ANGLE_START, SPEED_CIRCLE_ANGLE_END);
    drawNeedle(SPEED_CIRCLE_CENTER_X, SPEED_CIRCLE_CENTER_Y, SPEED_CIRCLE_RADIUS - 4, last_deg, 2,
               SSD1306_BLACK);
    drawNeedle(SPEED_CIRCLE_CENTER_X, SPEED_CIRCLE_CENTER_Y, SPEED_CIRCLE_RADIUS - 4, deg, 2,
               SSD1306_WHITE);

    lastVal = value;
}

void DashboardI2c::drawRPM(uint16_t value)
{
    static uint16_t lastVal = 0xFFFF;
    if (value == lastVal)
    {
        return;
    }

    int16_t last_deg = map(lastVal, RPM_MIN, RPM_MAX, RPM_CIRCLE_ANGLE_START, RPM_CIRCLE_ANGLE_END);
    int16_t deg = map(value, RPM_MIN, RPM_MAX, RPM_CIRCLE_ANGLE_START, RPM_CIRCLE_ANGLE_END);
    drawNeedle(RPM_CIRCLE_CENTER_X, RPM_CIRCLE_CENTER_Y, RPM_CIRCLE_RADIUS - 4, last_deg, 2,
               SSD1306_BLACK);
    drawNeedle(RPM_CIRCLE_CENTER_X, RPM_CIRCLE_CENTER_Y, RPM_CIRCLE_RADIUS - 4, deg, 2, SSD1306_WHITE);
    lastVal = value;
    display.display();
}

void DashboardI2c::drawFuel(uint16_t value)
{
    static uint16_t lastVal = 0xFFFF;
    if (value == lastVal)
    {
        return;
    }

    int16_t last_deg =
        map(lastVal, FUEL_MIN, FUEL_MAX, FUEL_CIRCLE_ANGLE_START, FUEL_CIRCLE_ANGLE_END);
    int16_t deg = map(value, FUEL_MIN, FUEL_MAX, FUEL_CIRCLE_ANGLE_START, FUEL_CIRCLE_ANGLE_END);
    drawNeedle(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, FUEL_CIRCLE_RADIUS - 4, last_deg, 1,
               SSD1306_BLACK);
    drawNeedle(FUEL_CIRCLE_CENTER_X, FUEL_CIRCLE_CENTER_Y, FUEL_CIRCLE_RADIUS - 4, deg, 1, SSD1306_WHITE);

    lastVal = value;
    display.display();
}

void DashboardI2c::drawAdBlue(uint16_t value)
{
    static uint16_t lastVal = 0xFFFF;
    if (value == lastVal)
    {
        return;
    }

    int16_t last_deg =
        map(lastVal, ADBLUE_MIN, ADBLUE_MAX, ADBLUE_CIRCLE_ANGLE_START, ADBLUE_CIRCLE_ANGLE_END);
    int16_t deg =
        map(value, ADBLUE_MIN, ADBLUE_MAX, ADBLUE_CIRCLE_ANGLE_START, ADBLUE_CIRCLE_ANGLE_END);
    drawNeedle(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, ADBLUE_CIRCLE_RADIUS - 4, last_deg, 1,
               SSD1306_BLACK);
    drawNeedle(ADBLUE_CIRCLE_CENTER_X, ADBLUE_CIRCLE_CENTER_Y, ADBLUE_CIRCLE_RADIUS - 4, deg, 1,
               SSD1306_WHITE);

    lastVal = value;
    display.display();
}

void DashboardI2c::drawPump(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t cols = sizeof(PUMP_ICON_I2C[0]);
    uint8_t rows = sizeof(PUMP_ICON_I2C) / cols;
    for (uint8_t i = 0; i < rows; i++)
    {
        for (uint8_t j = 0; j < cols; j++)
        {
            if (PUMP_ICON_I2C[i][j])
            {
              display.drawPixel(x + j, y + i, SSD1306_WHITE);
            }
        }
    }
    display.display();
}

void DashboardI2c::drawGear(char gear)
{
    static char lastVal = ' ';
    if (gear == lastVal)
    {
        return;
    }
    char str[2] = "\0";
    str[0] = gear;
    display.fillRect(SCREEN_WIDTH / 2 - 3, SCREEN_HEIGHT / 2 - 2, 6, 7, SSD1306_BLACK);

    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH / 2) - 2, SCREEN_HEIGHT / 2 - 2);
    display.print(str);
    
    lastVal = gear;
    display.display();
}

void DashboardI2c::drawVolt(float volt, float threshold)
{
    static float lastVal = '1';
    if (abs(volt - lastVal) < 0.1)
        return; // Prevent display from being triggered too frequently

    display.fillRect((SCREEN_WIDTH / 2) - 9, (SCREEN_HEIGHT / 2) - 15, 20, 8, SSD1306_BLACK);
    display.setCursor((SCREEN_WIDTH / 2) - 9, (SCREEN_HEIGHT / 2) - 15);
    display.print(volt,1);


    lastVal = volt;
    display.display();
}

// Draw logo
void DashboardI2c::drawLogo(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h)
{
    int row, col, buffidx = 0;
    for (row = x; row < h; row++)
    { // For each scanline...
        for (col = y; col < w; col++)
        { // For each pixel...
          //if (buffidx %2 ) display.drawPixel(col, row, SSD1306_WHITE);
          //else display.drawPixel(col, row, SSD1306_BLACK);
          display.drawPixel(col, row, pgm_read_word(bitmap + buffidx));
          //_tft.drawPixel(col, row, pgm_read_word(bitmap + buffidx));
          buffidx++;
        }
    }
    display.display();
}
// End of standard dashboard *********************************************************************************************************************

