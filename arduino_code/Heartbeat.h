/*
 * Heartbeat.h - Shared types, helpers and function prototypes.
 * Included by every .ino tab so the Arduino build (which concatenates all
 * tabs into one translation unit) always has declarations available.
 */
#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <Arduino.h>
#include "Config.h"

/* Status of the monitoring system */
enum Status {
  NO_SIGNAL = 0,
  LOW_RATE  = 1,
  NORMAL    = 2,
  HIGH_RATE = 3
};

/* Global state shared between modules */
extern int  g_currentBPM;
extern bool g_signalLost;
extern int  g_targetBPM;   /* knob setpoint in sim mode (-1 = n/a) */

/* Small float absolute value (avoids pulling in <math.h>) */
static inline float myAbs(float x) { return (x < 0.0f) ? -x : x; }

/* ---- Module prototypes ---- */
int  getPulseSample(void);                       /* PulseSensor.ino   */
int  computeBPM(int sample, unsigned long now);  /* BPMCalculator.ino */
Status evaluateStatus(int bpm, bool lost);       /* AlertSystem.ino   */
void   applyAlert(Status s);                     /* AlertSystem.ino   */
void   displayInit(void);                         /* DisplayModule.ino */
void   updateDisplay(int bpm, int setpoint, Status s); /* DisplayModule.ino */

#endif /* HEARTBEAT_H */
