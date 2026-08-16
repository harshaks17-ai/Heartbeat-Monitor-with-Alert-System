# Test Report — Heartbeat Monitor with Alert System

**Date:** project build
**Method:** Python port of firmware algorithm (`src/sim_bpm_test.py`) + Wokwi simulation.

## Automated Algorithm Tests (PC)

| Scenario | Target | Measured | Status | Result |
|---|---|---|---|---|
| Low BPM | 50 | 50 | LOW | PASS |
| Normal BPM | 75 | 75 | NORMAL | PASS |
| High BPM | 130 | 130 | HIGH | PASS |
| No signal | — | 0 | NO_SIGNAL | PASS |

All four core scenarios passed (tolerance ±4 BPM).

## Manual / Wokwi Checklist

| # | Test | Expected | Pass/Fail |
|---|---|---|---|
| 1 | Start sim, target 75 | OLED shows ~75, green LED, NORMAL | ☐ |
| 2 | Set target 50 | Red LED + buzzer, LOW | ☐ |
| 3 | Set target 130 | Red LED + buzzer, HIGH | ☐ |
| 4 | Serial Monitor | Prints BPM + status every 0.5 s | ☐ |
| 5 | Noise resilience | Stable BPM, no false spikes | ☐ |
| 6 | Real sensor (flag 0) | Tracks finger pulse | ☐ |

## Notes
- The refractory interval (`MIN_BEAT_INTERVAL_MS = 300`) caps detectable BPM at
  ~200 and rejects noise double-counting.
- Adaptive threshold tolerates baseline drift from the sensor.
