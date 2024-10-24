#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 * Modified: I2C Oled Monochrome SSD1306 128x32
 * http:// 
 *
 *
 *
 * original: dashboard.h - Library to drive a 80x160px color LCD as rc truck dashboard
 *
 * https://github.com/Gamadril/Rc_Engine_Sound_ESP32
 * MIT License
 */

#ifndef DASHBOARD_h
#define DASHBOARD_h

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define DEG2RAD 0.0174532925
#define SPEED_MIN 0
#define SPEED_MAX 120
#define RPM_MIN 0
#define RPM_MAX 500
#define FUEL_MIN 0
#define FUEL_MAX 100
#define ADBLUE_MIN 0
#define ADBLUE_MAX 100



#define SPEED_CIRCLE_CENTER_X 18
#define SPEED_CIRCLE_CENTER_Y (SCREEN_HEIGHT / 2) + 2
#define SPEED_CIRCLE_RADIUS 18
#define SPEED_NEEDLE_LENGTH 20
#define SPEED_CIRCLE_ANGLE_START -203 // -180 - 23
#define SPEED_CIRCLE_SEGMENTS 6
#define SPEED_CIRCLE_SEGMENT_ANGLE 37.9
#define SPEED_CIRCLE_ANGLE_END \
    SPEED_CIRCLE_ANGLE_START + SPEED_CIRCLE_SEGMENTS *SPEED_CIRCLE_SEGMENT_ANGLE


#define RPM_CIRCLE_CENTER_X SCREEN_WIDTH - 18
#define RPM_CIRCLE_CENTER_Y (SCREEN_HEIGHT / 2) + 2
#define RPM_CIRCLE_RADIUS 18 
#define RPM_CIRCLE_ANGLE_START -202 // -180 - 22
#define RPM_CIRCLE_SEGMENTS 8
#define RPM_CIRCLE_SEGMENT_ANGLE 28.3
#define RPM_CIRCLE_ANGLE_END RPM_CIRCLE_ANGLE_START + RPM_CIRCLE_SEGMENTS *RPM_CIRCLE_SEGMENT_ANGLE

#define FUEL_CIRCLE_CENTER_X 15
#define FUEL_CIRCLE_CENTER_Y 31 // SCREEN_HEIGHT / 2 + 10
#define FUEL_CIRCLE_RADIUS 8
#define FUEL_CIRCLE_ANGLE_START -202 // -180 - 22
#define FUEL_CIRCLE_SEGMENTS 4
#define FUEL_CIRCLE_SEGMENT_ANGLE 56
#define FUEL_CIRCLE_ANGLE_END \
    FUEL_CIRCLE_ANGLE_START + FUEL_CIRCLE_SEGMENTS *FUEL_CIRCLE_SEGMENT_ANGLE

#define ADBLUE_CIRCLE_CENTER_X SCREEN_WIDTH - 15
#define ADBLUE_CIRCLE_CENTER_Y 31
#define ADBLUE_CIRCLE_RADIUS 8
#define ADBLUE_CIRCLE_ANGLE_START -202 // -180 - 22
#define ADBLUE_CIRCLE_SEGMENTS 4
#define ADBLUE_CIRCLE_SEGMENT_ANGLE 56
#define ADBLUE_CIRCLE_ANGLE_END \
    ADBLUE_CIRCLE_ANGLE_START + ADBLUE_CIRCLE_SEGMENTS *ADBLUE_CIRCLE_SEGMENT_ANGLE

#define INDICATOR_LEFT_CENTER_X (SCREEN_WIDTH / 2) - 24
#define INDICATOR_LEFT_CENTER_Y 2

#define INDICATOR_RIGHT_CENTER_X (SCREEN_WIDTH / 2) + 15
#define INDICATOR_RIGHT_CENTER_Y 2


const uint8_t LEFT_INDICATOR_ICON_I2C[7][9] PROGMEM = {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0}
    };

const uint8_t RIGHT_INDICATOR_ICON_I2C[7][9] PROGMEM = {
    {0, 0, 0, 0, 0, 10, 0, 0 ,0},
    {0, 0, 0, 0, 0, 1, 1, 0 ,0},
    {1, 1, 1, 1, 1, 1, 1, 1 ,0},
    {1, 1, 1, 1, 1, 1, 1, 1 ,1},
    {1, 1, 1, 1, 1, 1, 1, 1 ,0},
    {0, 0, 0, 0, 0, 1, 1, 0 ,0},
    {0, 0, 0, 0, 0, 1, 0, 0 ,0}
    };


const uint8_t LOW_BEAM_ICON_I2C[7][13] PROGMEM = {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}};

const uint8_t HIGH_BEAM_ICON_I2C[7][13] PROGMEM = {
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0}};

const uint8_t FOG_LIGHT_ICON_I2C[7][13] PROGMEM = {
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0}};

const uint8_t PUMP_ICON_I2C[6][7] PROGMEM = {
    {0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 1},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 0}};

/**
 * @brief Class acts as a wrapper around TFT_eSPI routines to draw dashboard elements.
 *
 */
class DashboardI2c
{
public:
    /**
     * Default constructor
     */
    DashboardI2c();

    /**
     * Performs initialization of the display, rotates the coordinate system of the screen
     * (0,0) is the upper left corner if the connector is on the top.
     * Draws static dashboard elements and sets all values to 0.
     */
    void init(uint8_t value);

    /**
     * Sets the state of the left turn signal.
     * @param on - true to show the signal, false to deactivate it
     */
    void setLeftIndicator(boolean on);

    /**
     * Sets the state of the right turn signal.
     * @param on - true to show the signal, false to deactivate it
     */
    void setRightIndicator(boolean on);

    /**
     * Sets the state of the low beam lights.
     * @param on - true to show the indicator, false to deactivate it
     */
    void setLowBeamIndicator(boolean on);

    /**
     * Sets the state of the high beam lights.
     * @param on - true to show the indicator, false to deactivate it
     */
    void setHighBeamIndicator(boolean on);

    /**
     * Sets the state of the fog lights.
     * @param on - true to show the indicator, false to deactivate it
     */
    void setFogLightIndicator(boolean on);

    /**
     * Sets the speed to show by rotating the needle to the right position.
     * @param value - speed value between SPEED_MIN and SPEED_MAX
     */
    void setSpeed(uint16_t value);

    /**
     * Sets the RPM to show by rotating the needle to the right position.
     * @param value - RPM value between RPM_MIN and RPM_MAX
     */
    void setRPM(uint16_t value);

    /**
     * Sets the fuel level to show by rotating the needle to the right position.
     * @param value - fuel value between FUEL_MIN and FUEL_MAX
     */
    void setFuelLevel(uint16_t value);

    /**
     * Sets the AdBlue level to show by rotating the needle to the right position.
     * @param value - AdBlue value between ADBLUE_MIN and ADBLUE_MAX
     */
    void setAdBlueLevel(uint16_t value);

    /**
     * Sets the gear to show.
     * @param value - -1 for R, 0 for N, > 0 for gear number
     */
    void setGear(int8_t value);
    void setVolt(float value, float threshold);

private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    void drawFrame();
    void drawArc(int16_t cx, int16_t cy, int16_t r, int16_t deg_start, int16_t deg_end,
                 uint16_t color);
    void drawSep(int16_t cx, int16_t cy, uint16_t len, int16_t deg_pos, uint16_t r, uint16_t color);
    void drawNeedle(int16_t cx, int16_t cy, uint16_t len, int16_t deg_pos, uint16_t radius_holder,
                    uint16_t color_needle);
    void drawLeftIndicator(uint16_t color);
    void drawRightIndicator(uint16_t color);
    void drawLowBeamIndicator(uint16_t color);
    void drawHighBeamIndicator(uint16_t color);
    void drawFogLightIndicator(uint16_t color);
    void drawSpeed(uint16_t value);
    void drawRPM(uint16_t value);
    void drawFuel(uint16_t value);
    void drawAdBlue(uint16_t value);
    void drawPump(uint16_t x, uint16_t y, uint16_t color);
    void drawGear(char gear);
    void drawVolt(float volt, float threshold);

    void drawLogo(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h); //Logo
};

#endif
// End of standard dashboard *********************************************************************************************************************
