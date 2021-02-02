// Setup and resources
#include "hardware_config.h"
#include "res.h"

// Libraries
#include <Arduino.h>
#include "LedControl.h"
#include "U8g2lib.h"

// Display drivers
LedControl lc(LED_DIN, LED_CLK, LED0_CS, 2);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// Heart animation frame indices
int indices[6] = {0, 1, 2, 3, 2, 1};

// Resin count
unsigned short resin = 0;

const int FRAME_DELAY = 100;
const int FRAME_COUNT = 6;
const int DISPLAY_COUNT = 3;
const int ROW_COUNT = 8;

// Core logic

void setup()
{
    setupLEDs();
    setupLCD();
    delay(500);
    setupMatrices();
}

void loop()
{
    drawLCD();
    renderHeartAnimation();
    blinkLEDs();
    updateResinCount();
}

void setupLEDs()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
}

void setupLCD()
{
    u8g2.begin();
    u8g2.setFont(hl_alyx);
}

void setupMatrices()
{
    for (int i = 0; i < DISPLAY_COUNT; i++)
    {
        lc.shutdown(i, false);
        lc.setIntensity(i, 4);
        lc.clearDisplay(i);
    }
}

void drawLCD()
{
    u8g2.clearBuffer();
    u8g2.drawXBMP(ball_left, ball_top, ball_width, ball_height, ball_bits);
    if (resin > 9)
        u8g2.drawGlyph((LCD_WIDTH / 2) - 2, LCD_HEIGHT - FONT_OFFSET_Y, (resin / 10) + '0');
    else
        u8g2.drawGlyph((LCD_WIDTH / 2) - 2, LCD_HEIGHT - FONT_OFFSET_Y, ':');
    u8g2.drawGlyph(LCD_WIDTH - FONT_WIDTH, LCD_HEIGHT - FONT_OFFSET_Y, (resin % 10) + '0');
    u8g2.sendBuffer();
}

void renderHeartAnimation()
{
    for (int frame = 0; frame < FRAME_COUNT; frame++)
    {
        for (int display = 0; display < DISPLAY_COUNT; display++)
            for (int row = 0; row < ROW_COUNT; row++)
                lc.setRow(display, row, heart[indices[(frame + display) % FRAME_COUNT]][row]);
        delay(FRAME_DELAY);
    }
}

void blinkLEDs()
{
    digitalWrite(LED1, resin % 2 == 0 ? HIGH : LOW);
    digitalWrite(LED2, resin % 3 == 0 ? HIGH : LOW);
    digitalWrite(LED3, resin % 5 == 0 ? HIGH : LOW);
}

void updateResinCount()
{
    resin++;
    if (resin >= 100)
        resin = 0;
}