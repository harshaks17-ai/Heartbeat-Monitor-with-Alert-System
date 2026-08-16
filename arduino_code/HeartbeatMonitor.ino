/*
 * HeartbeatMonitor.ino - Main firmware (Arduino UNO / ESP32).
 *
 * Flow:
 *   read pulse sample -> detect beats -> compute BPM -> evaluate status
 *   -> drive buzzer/LEDs -> update OLED -> print to Serial Monitor.
 *
 * EDUCATIONAL PROTOTYPE - NOT A MEDICAL DEVICE.
 * Author: Harsha K S
 */
#include "Heartbeat.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for serial (ESP32) */ }

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED,   OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED,   LOW);

#if !USE_SIMULATED_PULSE
  pinMode(SENSOR_PIN, INPUT);
#endif

  displayInit();

  Serial.println("========================================");
  Serial.println(" Heartbeat Monitor with Alert System");
  Serial.println(" Educational prototype - NOT a medical device");
#if USE_SIMULATED_PULSE
  Serial.print(" Mode: SIMULATED pulse @ ");
  Serial.print(SIM_TARGET_BPM);
  Serial.println(" BPM");
#else
  Serial.println(" Mode: REAL Pulse Sensor on A0");
#endif
  Serial.println("========================================");
}

void loop() {
  int sample = getPulseSample();
  unsigned long now = millis();

  computeBPM(sample, now);
  Status s = evaluateStatus(g_currentBPM, g_signalLost);
  applyAlert(s);
  updateDisplay(g_currentBPM, g_targetBPM, s);

  /* Throttled Serial output (every 500 ms) */
  static unsigned long lastPrint = 0;
  if (now - lastPrint > 500) {
    lastPrint = now;
    Serial.print("BPM: ");
    Serial.print(g_currentBPM);
    if (g_targetBPM >= 0) { Serial.print("  Set: "); Serial.print(g_targetBPM); }
    Serial.print("  Status: ");
    switch (s) {
      case NORMAL:    Serial.println("NORMAL"); break;
      case LOW_RATE:  Serial.println("LOW");    break;
      case HIGH_RATE: Serial.println("HIGH");   break;
      default:        Serial.println("NO SIGNAL"); break;
    }
  }

  delay(4);   /* ~250 Hz sampling -> less beat-time jitter */
}
