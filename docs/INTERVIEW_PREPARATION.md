# Interview Preparation — Heartbeat Monitor with Alert System

Use this to explain the project confidently in viva/technical interviews.

## 30-Second Pitch
"I built an embedded heart-rate monitor on Arduino UNO. It samples a pulse
sensor, detects individual beats with an adaptive-threshold peak detector,
computes BPM as `60000 / inter-beat-interval` with a moving average, and raises
a buzzer + red-LED alert when BPM leaves a safe range, with a green LED for
normal. It runs in Wokwi via a synthetic waveform so it's fully demonstrable
without hardware. It's an educational prototype, not a medical device."

## Likely Questions & Answers

**Q: How do you compute BPM?**
A: Time between two consecutive R-wave/systolic peaks in milliseconds, then
`BPM = 60000 / interval`. I average the last 5 intervals to stabilize it.

**Q: How do you detect a beat from a noisy analog signal?**
A: I keep a slow EMA baseline and a decaying peak-hold, then set an adaptive
threshold at `baseline + 0.55*(peak - baseline)`. A rising cross above the
threshold (after a refractory period that rejects noise double-counting) is one
beat.

**Q: Why a refractory / minimum interval?**
A: To cap the maximum detectable rate (~200 BPM) and ignore noise spikes or the
smaller dicrotic wave so they don't create false beats.

**Q: Why not just map analog value to BPM?**
A: That assumes a fixed relationship between amplitude and rate, which isn't
true for a PPG waveform. Beat timing is the physically correct way.

**Q: What are the trade-offs of your approach?**
A: Adaptive threshold tolerates baseline drift but needs a few beats to
converge; moving average adds latency. For a clinical device you'd use
certified optical sensors, filtering, and validated algorithms.

**Q: How would you make it better / production-ready?**
A: Use MAX30102 (SpO₂ + perfusion), ESP32 for Wi-Fi/Bluetooth dashboard,
EEPROM-configurable thresholds, low-power sleep, and proper medical
certification + signal quality checks.

**Q: What embedded concepts did you use?**
A: ADC sampling, `millis()` timing (non-blocking), GPIO actuators, I2C OLED,
PWM/tone for buzzer, finite-state status logic, modular firmware design.

## Key Numbers to Remember
- `BPM = 60000 / ms_between_beats`
- 1000 ms → 60 BPM; 750 ms → 80 BPM; 600 ms → 100 BPM
- Demo thresholds: <60 LOW, 60–100 NORMAL, >100 HIGH (educational only)
- Sample rate ~100 Hz (`delay(10)`)
- UNO: 2 KB RAM, 32 KB flash (project used ~58% flash, ~41% RAM)

## Demo Flow for a Live Show
1. Open Wokwi, start sim → 75 BPM NORMAL, green LED.
2. Change `SIM_TARGET_BPM` to 50 → red LED + buzzer, LOW.
3. Change to 130 → red LED + buzzer, HIGH.
4. Show Serial Monitor + OLED.
