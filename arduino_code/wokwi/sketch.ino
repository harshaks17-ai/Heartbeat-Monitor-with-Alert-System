/*
 * Heartbeat Monitor with Alert System - SINGLE-FILE version (Wokwi friendly).
 * Fully self-contained: no external .h or extra .ino files required.
 * EDUCATIONAL PROTOTYPE - NOT A MEDICAL DEVICE.
 * Author: Harsha K S
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ---------------- Config ---------------- */
#define USE_SIMULATED_PULSE 1
#define SIM_TARGET_BPM 75
#define USE_POT_CONTROL 1
#define POT_PIN A1
#define POT_BPM_MIN 40
#define POT_BPM_MAX 180
#define SENSOR_PIN A0
#define BUZZER_PIN 8
#define GREEN_LED 7
#define RED_LED    6
#define LOW_THRESHOLD_BPM  60
#define HIGH_THRESHOLD_BPM 100
#define MIN_BEAT_INTERVAL_MS 300
#define BEAT_TIMEOUT_MS     3000
#define BPM_SMOOTH 0.5f

/* ---------------- Types ---------------- */
enum Status { NO_SIGNAL = 0, LOW_RATE = 1, NORMAL = 2, HIGH_RATE = 3 };

/* ---------------- Shared state ---------------- */
int  g_currentBPM = 0;
bool g_signalLost = true;
int  g_targetBPM  = -1;

/* ---------------- OLED ---------------- */
#define SCREEN_W 128
#define SCREEN_H 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

/* ---------------- Helpers ---------------- */
static inline float myAbs(float x) { return (x < 0.0f) ? -x : x; }

/* ---------------- Pulse acquisition ---------------- */
int getPulseSample(void) {
#if USE_SIMULATED_PULSE
  static unsigned long startMs = 0, lastMs = 0;
  static float phaseFrac = 0.0f;
  static float potSmoothed = -1.0f;
  if (startMs == 0) { startMs = millis(); lastMs = millis(); }

  unsigned long now = millis();
  unsigned long dt  = now - lastMs; lastMs = now;
  if (dt == 0) dt = 1;

  int target = SIM_TARGET_BPM;
#if USE_POT_CONTROL
  int raw = analogRead(POT_PIN);
  if (potSmoothed < 0) potSmoothed = raw;
  else potSmoothed = potSmoothed * 0.8f + raw * 0.2f;   // low-pass knob
  target = map((int)potSmoothed, 0, 1023, POT_BPM_MIN, POT_BPM_MAX);
#endif
  g_targetBPM = target;

  float period = 60000.0f / (float)target;
  phaseFrac += (float)dt / period;
  if (phaseFrac >= 1.0f) phaseFrac -= 1.0f;

  float v = 512.0f;
  float dx  = phaseFrac - 0.15f; if (myAbs(dx)  < 0.06f) v += 350.0f * (1.0f - myAbs(dx)  / 0.06f);
  float dx2 = phaseFrac - 0.40f; if (myAbs(dx2) < 0.06f) v += 120.0f * (1.0f - myAbs(dx2) / 0.06f);
  v += (float)(random(0, 17) - 8);
  return (int)constrain(v, 0, 1023);
#else
  g_targetBPM = -1;
  return analogRead(SENSOR_PIN);
#endif
}

/* ---------------- BPM calculation ---------------- */
static float baseline = 512.0f;
static int   peakHold = 512;
static unsigned long lastBeatTime = 0;
static bool  beatInProgress = false;

int computeBPM(int sample, unsigned long now) {
  baseline = baseline * 0.95f + sample * 0.05f;
  if (sample > peakHold) peakHold = sample;
  else peakHold = (int)((float)peakHold * 0.999f);
  int threshold = (int)(baseline + ((float)peakHold - baseline) * 0.55f);

  if (!beatInProgress) {
    if (sample > threshold &&
        (now - lastBeatTime) > (unsigned long)MIN_BEAT_INTERVAL_MS) {
      if (lastBeatTime > 0) {
        unsigned long interval = now - lastBeatTime;
        if (interval > 0 && interval < 2000) {
          int instBpm = (int)(60000.0f / (float)interval);
          if (g_currentBPM == 0) g_currentBPM = instBpm;
          else g_currentBPM = (int)((float)g_currentBPM * (1.0f - BPM_SMOOTH)
                                  + (float)instBpm * BPM_SMOOTH);
        }
      }
      lastBeatTime = now;
      beatInProgress = true;
      g_signalLost = false;
    }
  } else {
    if (sample < threshold - 25) beatInProgress = false;
  }
  if ((now - lastBeatTime) > (unsigned long)BEAT_TIMEOUT_MS) {
    g_signalLost = true; g_currentBPM = 0;
  }
  return g_currentBPM;
}

/* ---------------- Alert logic ---------------- */
Status evaluateStatus(int bpm, bool lost) {
  if (lost || bpm <= 0) return NO_SIGNAL;
  if (bpm <  LOW_THRESHOLD_BPM) return LOW_RATE;
  if (bpm >  HIGH_THRESHOLD_BPM) return HIGH_RATE;
  return NORMAL;
}
void applyAlert(Status s) {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED,   LOW);
  noTone(BUZZER_PIN);
  switch (s) {
    case NORMAL:    digitalWrite(GREEN_LED, HIGH); break;
    case LOW_RATE:
    case HIGH_RATE: digitalWrite(RED_LED, HIGH); tone(BUZZER_PIN, 2000); break;
    default: break;
  }
}

/* ---------------- Display ---------------- */
void displayInit(void) {
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) return;
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
  if (s == NO_SIGNAL) display.println("-- BPM");
  else { display.print(bpm); display.println(" BPM"); }
  display.setTextSize(1);
  display.setCursor(0, 46);
  if (setpoint >= 0) { display.print("Set: "); display.print(setpoint); display.print("  "); }
  switch (s) {
    case NORMAL:    display.println("NORMAL"); break;
    case LOW_RATE:  display.println("LOW !");  break;
    case HIGH_RATE: display.println("HIGH !"); break;
    default:        display.println("NO SIGNAL"); break;
  }
  display.display();
}

/* ---------------- Main ---------------- */
void setup() {
  Serial.begin(9600);
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
  delay(4);
}
