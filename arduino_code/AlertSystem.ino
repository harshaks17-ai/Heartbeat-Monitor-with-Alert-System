/*
 * AlertSystem.ino - Threshold evaluation + actuator control.
 *
 * Decision logic (educational demonstration only, NOT medical advice):
 *   NO_SIGNAL : no beat detected for BEAT_TIMEOUT_MS
 *   LOW_RATE  : BPM <  LOW_THRESHOLD_BPM   -> Red LED + Buzzer
 *   HIGH_RATE : BPM >  HIGH_THRESHOLD_BPM  -> Red LED + Buzzer
 *   NORMAL    : LOW <= BPM <= HIGH         -> Green LED
 */
#include "Heartbeat.h"

Status evaluateStatus(int bpm, bool lost) {
  if (lost || bpm <= 0) return NO_SIGNAL;
  if (bpm <  LOW_THRESHOLD_BPM) return LOW_RATE;
  if (bpm >  HIGH_THRESHOLD_BPM) return HIGH_RATE;
  return NORMAL;
}

void applyAlert(Status s) {
  /* Default: everything off */
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED,   LOW);
  noTone(BUZZER_PIN);

  switch (s) {
    case NORMAL:
      digitalWrite(GREEN_LED, HIGH);
      break;

    case LOW_RATE:
    case HIGH_RATE:
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER_PIN, 2000);   /* continuous 2 kHz alert tone */
      break;

    case NO_SIGNAL:
    default:
      /* Both LEDs off, buzzer silent */
      break;
  }
}
