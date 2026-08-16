/*
 * BPMCalculator.ino - Heartbeat detection + BPM calculation module.
 *
 * Algorithm (works for both synthetic and real Pulse Sensor signals):
 *   1. Maintain a slow EMA baseline of the signal.
 *   2. Track a decaying peak-hold value.
 *   3. Adaptive threshold = baseline + 0.55 * (peakHold - baseline).
 *   4. When the signal crosses the threshold upward (and a minimum beat
 *      interval has passed), register a beat and record the time.
 *   5. BPM = 60000 / (ms between consecutive beats), smoothed with a
 *      per-beat EMA (BPM_SMOOTH) for fast yet stable response.
 */
#include "Heartbeat.h"

/* Internal state */
static float baseline     = 512.0f;
static int   peakHold     = 512;
static unsigned long lastBeatTime = 0;
static bool  beatInProgress = false;

/* Shared globals */
int  g_currentBPM = 0;
bool g_signalLost = true;

int computeBPM(int sample, unsigned long now) {
  /* Slow baseline tracking */
  baseline = baseline * 0.95f + sample * 0.05f;

  /* Peak-hold with slow decay */
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
    /* Hysteresis: wait until signal falls well below threshold */
    if (sample < threshold - 25) beatInProgress = false;
  }

  /* Signal-loss detection */
  if ((now - lastBeatTime) > (unsigned long)BEAT_TIMEOUT_MS) {
    g_signalLost = true;
    g_currentBPM = 0;
  }

  return g_currentBPM;
}
