# ❤️ Heartbeat Monitor with Alert System

> **Embedded Systems Course Project** — Arduino / ESP32 based heart-rate monitoring prototype with BPM calculation, OLED display, and threshold-based buzzer/LED alerts.

[![Embedded](https://img.shields.io/badge/embedded-C/Arduino-blue)](https://www.arduino.cc/)
[![Sim](https://img.shields.io/badge/simulation-Wokwi-green)](https://wokwi.com)
![License](https://img.shields.io/badge/license-MIT-green)

---

## ⚠️ Medical Disclaimer

**This is an educational embedded-systems prototype. It is NOT a medical device.**
Readings are produced by hobby-grade sensors and are **approximate only**. They
must **not** be used for diagnosis, treatment, or any medical decision. Clinical
heart-rate interpretation depends on age, health, activity, and context, and
should be performed by qualified professionals using certified equipment.

---

## 📌 Overview

A microcontroller continuously samples a pulse sensor, detects individual
heartbeats, computes Beats Per Minute (BPM), and compares it against
configurable educational thresholds. The system shows the live BPM on an OLED
display and raises a buzzer + red-LED alert when the rate is too low or too high;
a green LED indicates a normal range.

| Feature | Description |
|---|---|
| Real-time pulse acquisition | Analog pulse sensor / synthetic waveform |
| Beat detection | Adaptive-threshold peak detection |
| BPM calculation | `BPM = 60000 / beat_interval`, moving average |
| Display | SSD1306 OLED (128×64, I2C) |
| Alerts | Buzzer + Red LED (abnormal), Green LED (normal) |
| Simulation | Deterministic Wokwi virtual simulation (no hardware needed) |
| Real hardware | Flip one flag to use a physical Pulse Sensor |

---

## 🧰 Components Used

| Component | Role |
|---|---|
| Arduino UNO (or ESP32) | Microcontroller / processing |
| Pulse Sensor (or MAX30102) | Heart-rate signal source |
| SSD1306 OLED (I2C) | Visual BPM + status output |
| Active Buzzer | Audible alert |
| Green LED | Normal-status indicator |
| Red LED | Alert-status indicator |
| 2× 220 Ω resistors | LED current limiting |
| Breadboard + jumper wires | Interconnections |
| USB cable / 5V supply | Power |

---

## 🚀 Quick Start (Wokwi — no hardware required)

**Use the ready-made folder `arduino_code/wokwi/`** (it contains exactly one
self-contained `sketch.ino` + `diagram.json` + `libraries.txt`, no extra files):

1. Go to https://wokwi.com → **New Project** → **Arduino Uno**.
2. Drag the **`arduino_code/wokwi/`** folder (or its 3 files) into the project.
3. Press **▶ Start**.

That's it — no `Heartbeat.h`, no duplicate `setup()`. The OLED shows `BPM:` and
`Set:`; turn the **potentiometer** to change the heart rate live (red LED +
buzzer outside 60–100 BPM).

> The modular multi-file version (for GitHub / PlatformIO / VS Code) lives in
> `arduino_code/` directly. Both are behavior-identical and compile cleanly.
3. The OLED shows `BPM: xx` plus a `Set: xx` line; **Serial Monitor** prints
   `BPM: xx  Set: xx  Status: ...`.
4. **Turn the potentiometer** ("regulator") in the circuit to set the heart
   rate live — left ≈ 40 BPM (LOW), middle ≈ 110, right ≈ 180 (HIGH). The
   red LED + buzzer trigger outside 60–100 BPM; green LED inside.
   - Keyboard: click the knob, then ←/→ (fine), PgUp/PgDn (coarse),
     Home/End (ends).

> Prefer a fixed rate instead? In `Config.h` set `USE_POT_CONTROL 0` and edit
> `SIM_TARGET_BPM`. The simulation uses a deterministic synthetic pulse so
> alerts are always demonstrable. For a *real* Pulse Sensor, set
> `USE_SIMULATED_PULSE 0`.

---

## 🔧 Run in VS Code (Arduino extension)

> The firmware was compiled and verified with the official AVR toolchain
> (Arduino Uno: Flash 58 %, RAM 41 %).

1. Install **VS Code** + the **Arduino** extension (Microsoft).
2. Open the folder **`arduino_code/`** in VS Code.
3. <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd> → `Arduino: Board Config` →
   select **Arduino Uno**.
4. <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd> → `Arduino: Library Manager` →
   install **Adafruit SSD1306** and **Adafruit GFX Library**.
5. Open `HeartbeatMonitor.ino` and click **Verify** (✔) then **Upload** (→),
   or use the Command Palette `Arduino: Upload`.
6. <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd> → `Arduino: Open Serial Monitor`
   (9600 baud) to see live BPM.
7. To demo without hardware keep `USE_SIMULATED_PULSE 1` (edit `SIM_TARGET_BPM`
   in `Config.h`: 50 LOW, 75 NORMAL, 130 HIGH). For a real sensor set it to `0`.

## 🔧 Real Hardware (same as above, then)

1. In `Config.h` set `USE_SIMULATED_PULSE 0`.
2. Wire components per [`circuit_diagram/`](circuit_diagram/).
3. Select board (Arduino UNO) + COM port → **Upload**.
4. Open Serial Monitor (9600 baud), place a finger on the sensor, read BPM.

---

## 🗂️ Folder Structure

```
Heartbeat-Monitor-Alert-Embedded-System/
├── arduino_code/      # Wokwi-ready Arduino project (diagram.json + .ino)
├── src/               # Python validation harness (sim_bpm_test.py)
├── circuit_diagram/   # Wiring instructions + ASCII diagram
├── data/              # Sample Serial Monitor logs (normal/low/high)
├── outputs/           # Generated output artifacts
├── screenshots/       # Proof screenshots (checklist in file)
├── reports/           # Test report
├── docs/              # Full 19-section project documentation
├── simulation/        # Virtual-simulation notes
├── README.md
└── .gitignore
```

---

## 📚 Documentation

Full theory, industry relevance, architecture, BPM math, test strategy, GitHub
strategy, interview prep and more: **[docs/PROJECT_DOCUMENTATION.md](docs/PROJECT_DOCUMENTATION.md)**

## 🧪 Testing

Algorithm validated offline with `src/sim_bpm_test.py` (LOW/NORMAL/HIGH/NO-SIGNAL
all PASS). Test cases: **[reports/test_report.md](reports/test_report.md)**

## 📸 Proof Checklist

See **[screenshots/README.md](screenshots/README.md)** for the exact list of
screenshots to capture for your GitHub proof.

---

## 🎓 Learning Outcomes

- Analog sensor acquisition & ADC
- Real-time embedded signal processing (peak detection)
- Timing with `millis()` / sampling
- Threshold decision logic
- Actuator control (buzzer, LEDs)
- I2C OLED display interfacing
- Simulation-driven embedded development

---

## 👤 Author

Embedded Systems Student — built as coursework proof of work.
*`Heartbeat Monitor with Alert System` — educational prototype, not a medical device.*
