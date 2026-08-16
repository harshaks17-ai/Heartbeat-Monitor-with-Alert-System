/*
 * PulseSensor.ino - Signal acquisition module.
 *
 * In simulation mode it produces a smooth synthetic PPG (photoplethysmograph)
 * style waveform: a sharp systolic peak followed by a smaller dicrotic wave,
 * with mild noise. The shape mirrors what a real Pulse Sensor outputs, so the
 * exact same beat-detection pipeline is exercised in both modes.
 *
 * When USE_POT_CONTROL is enabled, a potentiometer (Wokwi "regulator") sets
 * the target BPM in real time; the reading is low-pass filtered for stability.
 *
 * In real-hardware mode it simply reads the analog Pulse Sensor pin.
 */
#include "Heartbeat.h"

int g_targetBPM = -1;

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
  int raw = analogRead(POT_PIN);                       // 0..1023 from knob
  if (potSmoothed < 0) potSmoothed = raw;
  else potSmoothed = potSmoothed * 0.8f + raw * 0.2f;  // low-pass the knob
  target = map((int)potSmoothed, 0, 1023, POT_BPM_MIN, POT_BPM_MAX);
#endif
  g_targetBPM = target;

  float period = 60000.0f / (float)target;
  phaseFrac += (float)dt / period;
  if (phaseFrac >= 1.0f) phaseFrac -= 1.0f;

  float v = 512.0f;
  float dx  = phaseFrac - 0.15f; if (myAbs(dx)  < 0.06f) v += 350.0f * (1.0f - myAbs(dx)  / 0.06f);
  float dx2 = phaseFrac - 0.40f; if (myAbs(dx2) < 0.06f) v += 120.0f * (1.0f - myAbs(dx2) / 0.06f);
  v += (float)(random(0, 17) - 8);                    // smaller noise -> less jitter
  return (int)constrain(v, 0, 1023);
#else
  g_targetBPM = -1;
  return analogRead(SENSOR_PIN);
#endif
}
