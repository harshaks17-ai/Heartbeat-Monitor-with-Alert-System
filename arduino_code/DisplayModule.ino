/*
 * DisplayModule.ino - OLED (SSD1306, 128x64, I2C) output module.
 *
 * Shows the current BPM, the knob "Set:" point (sim regulator), and status.
 * Falls back gracefully if the display is not detected.
 */
#include "Heartbeat.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_W 128
#define SCREEN_H 64
#define OLED_ADDR 0x3C

static Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

void displayInit(void) {
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    /* Display not found - continue without it */
    return;
  }
  display.clearDisplay();
  display.display();
}

void updateDisplay(int bpm, int setpoint, Status s) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Heartbeat Monitor");

  display.setTextSize(2);
  display.setCursor(0, 18);
  if (s == NO_SIGNAL) {
    display.println("-- BPM");
  } else {
    display.print(bpm);
    display.println(" BPM");
  }

  display.setTextSize(1);
  display.setCursor(0, 46);
  if (setpoint >= 0) {
    display.print("Set: ");
    display.print(setpoint);
    display.print("  ");
  }
  switch (s) {
    case NORMAL:    display.println("NORMAL"); break;
    case LOW_RATE:  display.println("LOW !");  break;
    case HIGH_RATE: display.println("HIGH !"); break;
    case NO_SIGNAL: display.println("NO SIGNAL"); break;
  }
  display.display();
}
