# Heartbeat Monitor with Alert System — Full Project Documentation

> **Educational embedded-systems prototype. NOT a medical device.**
> Readings from hobby-grade sensors are approximate and must not be used for
> medical decisions.

---

## 1️⃣ Project Explanation

### What is a Heartbeat Monitor with Alert System?
A small embedded device that continuously measures a person's heart rate
(Beats Per Minute, BPM) using a pulse sensor and warns the user when the rate
leaves a safe demonstration range.

### What problem does it solve?
It gives **low-cost, real-time** feedback on heart activity. In a training or
monitoring context it can draw attention to abnormal rates so a person can
rest, seek help, or check the sensor. As an educational build it demonstrates
the full embedded pipeline: sense → process → decide → actuate.

### What is heart rate / BPM?
BPM = the number of heartbeats in one minute. A resting adult is often around
60–100 BPM, but this varies widely by age, fitness, and activity. **This
project uses illustrative thresholds only.**

### How a pulse sensor detects heartbeat signals
A Pulse Sensor is a reflective photoplethysmograph (PPG): an LED shines light
into the skin and a photodiode measures how much is absorbed by blood. With
each heartbeat, blood volume rises, changing the reflected light — producing a
waveform with a sharp **systolic peak** and a smaller **dicrotic wave**.

### How a microcontroller processes pulse data
The MCU samples the sensor (analog → ADC), tracks a baseline, and detects the
rise of each beat. It measures the time between beats and converts it to BPM.

### How BPM is calculated
`BPM = 60000 / (ms between two consecutive beats)`. Averaging several beats
stabilises the reading.

### How abnormal threshold alerts are generated
The computed BPM is compared to `LOW_THRESHOLD_BPM` and `HIGH_THRESHOLD_BPM`.
Out-of-range → red LED + buzzer; in-range → green LED.

### How this demonstrates embedded concepts
Real-time sampling, ADC, timing, signal filtering, decision logic, actuator
control, and display interfacing — all on a resource-limited MCU.

### Simple Explanation
The device "listens" to your pulse, counts beats, shows the number on a screen,
and beeps if your heart is beating too slowly or too quickly.

### Technical Explanation
A pulse waveform is acquired via ADC, an adaptive-threshold peak detector
identifies systolic peaks, inter-beat intervals are timed with `millis()`,
BPM is derived and smoothed, then a finite-status state machine drives
indicators.

### Workflow
```
Pulse Sensor → Analog/Digital Signal → Microcontroller → Heartbeat Detection
→ BPM Calculation → Threshold Comparison → LCD/OLED Display
→ Buzzer / LED Alert
```

---

## 2️⃣ Industry Relevance

Similar monitoring concepts power:
- **Wearable fitness devices** (step + heart-rate bands)
- **Patient monitoring systems** in hospitals (bedside vital signs)
- **Smart watches** (optical HR via MAX3010x)
- **Sports monitoring** (zone training)
- **Rehabilitation devices** (recovery tracking)
- **Health prototypes** and **remote/telehealth monitoring**

Technical value demonstrated:
- Real-time sensor acquisition
- Basic signal processing (filtering, peak detection)
- Embedded decision logic
- Display interfacing
- Alert generation
- Low-cost monitoring prototype

---

## 3️⃣ Tech Stack / Hardware Options

### Option A — Easy
- Arduino UNO, Pulse Sensor, Serial Monitor, LED/Buzzer
- **Difficulty:** low · **Output:** text + basic alert · **Hardware:** optional (simulate)

### Option B — Recommended ⭐
- Arduino UNO / ESP32, Pulse Sensor, 16×2 LCD or OLED, Buzzer, Red/Green LEDs
- **Difficulty:** medium · **Output:** visual BPM + clear alerts
- **Hardware:** optional (fully simulatable in Wokwi)

### Option C — Advanced
- ESP32, MAX30102, OLED, data logging, Bluetooth/Wi-Fi dashboard, cloud
- **Difficulty:** high · **Output:** connected monitoring

**Selected for students:** **Option B** — best balance of industry relevance
and beginner-friendliness, and fully works in virtual simulation.

---

## 4️⃣ Embedded System Concepts Used

| Concept | Why it is used |
|---|---|
| Microcontroller | Central processing of sensor data |
| GPIO | Drive buzzer and LEDs |
| ADC / analog input | Read the analog pulse waveform |
| Pulse Sensor / MAX30102 | Heart-rate signal source |
| Sampling | Capture the waveform at a fixed rate |
| Peak detection | Identify individual beats |
| BPM calculation | Convert intervals to a meaningful number |
| Timer / `millis()` | Measure beat intervals without blocking |
| Threshold comparison | Decide normal vs abnormal |
| LCD / OLED | Human-readable output |
| Buzzer / LED | Feedback / alert |
| Embedded C / Arduino | Firmware implementation |
| Serial communication | Debugging and logging |
| Data logging (optional) | Record trends |

---

## 5️⃣ Hardware Components

- **Arduino UNO / ESP32** — runs firmware, processes signal.
- **Pulse Sensor / MAX30102** — outputs analog PPG waveform.
- **16×2 LCD / OLED** — shows BPM + status.
- **Buzzer** — audible alert when abnormal.
- **Green LED** — normal status.
- **Red LED** — alert status.
- **Breadboard / jumper wires** — connections.
- **Power supply (USB/5V)** — powers the system.

Each component's input/output and connection role are detailed in the circuit
section below.

---

## 6️⃣ Project Architecture

### Text Architecture
```
[ Pulse Sensor ] --(A0)--> [ MCU ]
                              |
              +---------------+---------------+
              |                               |
        [ Beat Detection ]            [ Display Task ]
              |                               |
        [ BPM Calculator ]            [ OLED / Serial ]
              |
        [ Threshold Logic ]
              |
        +-------+-------+
        |       |       |
     Green    Red     Buzzer
      LED     LED
```

### Input / Output Table
| I/O | Source / Destination | Meaning |
|---|---|---|
| IN | Pulse Sensor → A0 | Raw pulse waveform |
| IN | (optional) Button | Reset / recalibrate |
| OUT | OLED (I2C) | BPM + status |
| OUT | Green LED (D7) | Normal |
| OUT | Red LED (D6) | Alert |
| OUT | Buzzer (D8) | Audible alert |
| OUT | Serial | Debug/log |

### Heartbeat Calculation Flow
1. Sample sensor every ~10 ms.
2. Update baseline (EMA) and peak-hold.
3. Threshold = baseline + 0.55×(peak−baseline).
4. Rising cross above threshold (after refractory) = beat.
5. `interval = now − lastBeat`; `BPM = 60000/interval`.
6. Moving average of last N beats.

### Threshold Logic Table
| Condition | Status | Green | Red | Buzzer |
|---|---|---|---|---|
| No beat > 3 s | NO SIGNAL | off | off | off |
| BPM < 60 | LOW | off | on | on |
| 60 ≤ BPM ≤ 100 | NORMAL | on | off | off |
| BPM > 100 | HIGH | off | on | on |

*(Thresholds are educational demonstration values, not medical criteria.)*

---

## 7️⃣ Implementation Plan

| Phase | Objective | Key Tasks | Verify |
|---|---|---|---|
| 1 | Environment setup | Install IDE/PlatformIO, libraries | Blink works |
| 2 | Pulse interfacing | Wire sensor to A0 | `analogRead` varies |
| 3 | Raw signal | Print raw samples | Waveform visible |
| 4 | Beat detection | Adaptive threshold | Peaks counted |
| 5 | BPM calc | Interval → BPM | `60000/Δt` |
| 6 | Display | OLED shows BPM | Text renders |
| 7 | Threshold logic | Compare BPM | Status changes |
| 8 | Buzzer/LED | Actuators | Alert on/off |
| 9 | Integration | Full system | End-to-end |
| 10 | Testing | Calibrate thresholds | Stable BPM |
| 11 | Simulation | Wokwi run | Virtual demo |
| 12 | GitHub | Document & push | Repo complete |

Each phase's common mistakes: wrong pin, missing `Wire.begin()`, forgetting
`display.display()`, buzzer always on (no `noTone`). Verify with Serial prints.

---

## 8️⃣ Folder Structure

See repository root. Each folder:
- `arduino_code/` — runnable firmware + Wokwi diagram
- `src/` — Python validation harness
- `circuit_diagram/` — wiring
- `data/` — sample logs
- `outputs/` — artifacts
- `screenshots/` — proof
- `reports/` — test report
- `docs/` — this documentation
- `simulation/` — sim notes

---

## 9️⃣ Circuit Diagram (Wokwi / Real)

### Arduino ↔ Pulse Sensor (real hardware only)
> Wokwi has **no built-in pulse-sensor part**, so the virtual circuit omits it
> and uses the firmware's synthetic waveform. Use this wiring on real hardware.

| Arduino | Pulse Sensor |
|---|---|
| 5V | VCC |
| GND | GND |
| A0 | SIG |

### Arduino ↔ Buzzer (active)
| Arduino | Buzzer |
|---|---|
| D8 | + (pin 1) |
| GND | − (pin 2) |

### Arduino ↔ LEDs (with 220 Ω resistor)
| Arduino | Resistor | LED |
|---|---|---|
| D7 | R → LED anode | Green cathode → GND |
| D6 | R → LED anode | Red cathode → GND |

### Arduino ↔ OLED (I2C)
| Arduino | OLED |
|---|---|
| 5V | VCC |
| GND | GND |
| A4 | SDA |
| A5 | SCL |

---

## 🔟 Full Source Code

The firmware is modular:
- `PulseSensor.ino` — acquisition (real or synthetic)
- `BPMCalculator.ino` — beat detection + BPM
- `AlertSystem.ino` — threshold + actuators
- `DisplayModule.ino` — OLED
- `HeartbeatMonitor.ino` — main loop
- `Config.h` / `Heartbeat.h` — configuration & shared types

**Key functions**
- `getPulseSample()` — returns the current pulse value.
- `computeBPM(sample, now)` — detects beats, returns smoothed BPM.
- `evaluateStatus(bpm, lost)` — maps BPM to a status.
- `applyAlert(status)` — drives buzzer/LEDs.
- `updateDisplay(bpm, status)` — renders OLED.

> Full code is in `arduino_code/`. To change demo scenario edit `SIM_TARGET_BPM`
> in `Config.h` (50 = LOW, 75 = NORMAL, 130 = HIGH).

---

## 1️⃣1️⃣ Virtual Simulation (Wokwi)

1. Create an Arduino UNO project in Wokwi.
2. Add OLED (`board-ssd1306`), Buzzer, 2 LEDs + 2 resistors (use `diagram.json`).
3. Wire as in section 9 (note: the Pulse Sensor is real-hardware only).
4. Paste the `.ino` files from `arduino_code/`.
5. Set `USE_SIMULATED_PULSE 1` (default) — the firmware generates a synthetic pulse.
6. Press **Start**.
7. **Turn the potentiometer** (the "regulator") to set the heart rate live.
   The OLED shows `BPM:` (measured) and `Set:` (knob target); Serial Monitor
   prints both. Left ≈ 40 (LOW), middle ≈ 110, right ≈ 180 (HIGH).
8. LOW: knob left → red LED + buzzer, `Status: LOW`.
9. HIGH: knob right → red LED + buzzer, `Status: HIGH`.
10. NORMAL: mid-range → green LED, `Status: NORMAL`.
11. Confirm Serial Monitor + OLED match the knob position.

**Serial Monitor** shows: `BPM: 75  Status: NORMAL`.
**OLED** shows BPM + status.
**Capture:** simulation circuit, normal/low/high states, Serial output.

---

## 1️⃣2️⃣ BPM Calculation

`BPM = 60000 / time_between_beats_ms`

- 1000 ms → 60 BPM
- 750 ms → 80 BPM
- 600 ms → 100 BPM

Averaging multiple beats improves stability. Noisy signals create false peaks
→ the refractory (`MIN_BEAT_INTERVAL_MS`) and adaptive threshold reject them.
**Project thresholds are demonstration settings, not diagnosis.**

---

## 1️⃣3️⃣ Alert Logic

| BPM | Display | LED | Buzzer |
|---|---|---|---|
| < 60 | LOW | Red ON | ON |
| 60–100 | NORMAL | Green ON | OFF |
| > 100 | HIGH | Red ON | ON |

Clinical interpretation varies by individual and context; **do not treat these
thresholds as medical advice.**

---

## 1️⃣4️⃣ Testing Strategy

| Test | Input | Expected | Pass/Fail |
|---|---|---|---|
| No pulse | silent | NO SIGNAL | status NO_SIGNAL |
| Normal | 75 BPM | NORMAL, green | BPM≈75 |
| Low | 50 BPM | LOW, red+buzz | BPM≈50 |
| High | 130 BPM | HIGH, red+buzz | BPM≈130 |
| Noise | jittery signal | stable BPM | no false spikes |
| Disconnect | flatline | NO SIGNAL | alert clears |
| Buzzer | abnormal | audible | on/off correct |
| LED | per status | correct color | matches |
| Display | any | BPM + status | renders |

Automated via `src/sim_bpm_test.py` (all PASS).

---

## 1️⃣5️⃣ How to Run

**Real hardware:** IDE → board → COM → libraries → `USE_SIMULATED_PULSE 0` →
upload → Serial Monitor → finger on sensor.

**Virtual:** Wokwi → load `diagram.json` + `.ino` → Start → change
`SIM_TARGET_BPM`.

Sample Serial:
```
BPM: 75  Status: NORMAL
BPM: 75  Status: NORMAL
```
LED: green for normal, red for alert. Buzzer: sounds only on alert.

---

## 1️⃣6️⃣ GitHub Upload Strategy

**Repo name:** `Heartbeat-Monitor-Alert-Embedded-System`
**Description:** Embedded systems-based heartbeat monitoring prototype with BPM
calculation, display output, and threshold-based alerts using Arduino/ESP32.
**Tags:** `embedded-systems`, `arduino`, `esp32`, `pulse-sensor`,
`heartbeat-monitor`, `biomedical`, `embedded-c`, `sensors`, `oled`,
`automation`.

```bash
git init
git add .
git commit -m "Initial: Heartbeat Monitor with Alert System (Arduino + Wokwi)"
git branch -M main
git remote add origin <your-repo-url>
git push -u origin main
```

---

## 1️⃣7️⃣ README Generation

A complete README is provided at the repository root (title, overview, problem
statement, disclaimer, components, concepts, architecture, wiring, BPM math,
folder structure, install, simulation, run, screenshots, tests, limitations,
future work, learning outcomes, author).

---

## 1️⃣8️⃣ Proof Building Strategy (Day-wise)

| Day | Work | Commit | Proof |
|---|---|---|---|
| 1 | Setup + plan | `chore: project scaffold` | folder tree |
| 2 | Sensor interfacing | `feat: pulse acquisition` | raw signal |
| 3 | Beat detection | `feat: beat detection` | peaks |
| 4 | BPM calc | `feat: BPM calculation` | BPM output |
| 5 | OLED | `feat: OLED display` | display |
| 6 | Alerts | `feat: buzzer/LED alerts` | alert states |
| 7 | Sim + test | `test: simulation + tests` | sim screens |
| 8 | Docs | `docs: README + report` | GitHub page |

---

## 1️⃣9️⃣ Screenshots / Proof Checklist

See `screenshots/README.md`: folder structure, circuit, normal/low/high states,
LED colors, buzzer, OLED, Serial Monitor, source, GitHub preview.

---

## 🚧 Limitations
- Hobby-grade accuracy; motion artifacts; not medical grade.
- Single demonstrative threshold set.
- Synthetic mode is for learning, not real physiology.

## 🔮 Future Improvements
- MAX30102 SpO₂ + perfusion.
- ESP32 Wi-Fi/HTTP dashboard (ThingSpeak).
- EEPROM threshold configuration / buttons.
- Bluetooth mobile alert.
- Battery + low-power sleep.

## 🎓 Learning Outcomes
See README.
