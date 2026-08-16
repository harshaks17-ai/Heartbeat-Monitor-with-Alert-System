#ifndef CONFIG_H
#define CONFIG_H

/* 1. Simulation vs Real Hardware */
#define USE_SIMULATED_PULSE 1
#define SIM_TARGET_BPM 75

/* 1b. Manual regulator (potentiometer) - Wokwi sim only */
#define USE_POT_CONTROL 1      // 1 = knob sets BPM; 0 = fixed SIM_TARGET_BPM
#define POT_PIN A1
#define POT_BPM_MIN 40         // knob left  -> 40 BPM (LOW alert)
#define POT_BPM_MAX 180        // knob right -> 180 BPM (HIGH alert)

/* 2. Pin assignment (Arduino UNO) */
#define SENSOR_PIN A0
#define BUZZER_PIN 8
#define GREEN_LED  7
#define RED_LED    6

/* 3. Educational alert thresholds (NOT medical advice) */
#define LOW_THRESHOLD_BPM  60
#define HIGH_THRESHOLD_BPM 100

/* 4. Algorithm tuning */
#define MIN_BEAT_INTERVAL_MS 300
#define BEAT_TIMEOUT_MS     3000
#define BPM_SMOOTH 0.5f       // per-beat EMA (higher = faster, a bit noisier)

#endif
