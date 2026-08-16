# Virtual Simulation (Wokwi)

The project is fully simulatable without hardware.

## Steps
1. Go to https://wokwi.com and create an **Arduino UNO** project.
2. Upload the contents of `arduino_code/`: `diagram.json` + all `.ino` files.
3. Ensure `libraries.txt` lists `Adafruit SSD1306` and `Adafruit GFX Library`.
4. Press **Start Simulation** (▶).
5. Open **Serial Monitor** (bottom panel).

## Changing the demo scenario
Edit `arduino_code/Config.h`:
- `USE_SIMULATED_PULSE 1` — use the built-in synthetic waveform (default).
- `SIM_TARGET_BPM 50`  — LOW alert
- `SIM_TARGET_BPM 75`  — NORMAL
- `SIM_TARGET_BPM 130` — HIGH alert

## About the Pulse Sensor in simulation
Wokwi does **not** provide a built-in `pulse-sensor` part, so the simulation
uses the deterministic **synthetic waveform** generated inside the firmware
(`USE_SIMULATED_PULSE 1`). A **potentiometer** on **A1** acts as the "regulator"
that sets the target BPM in real time (`USE_POT_CONTROL 1`). The OLED shows both
the measured `BPM:` and the knob `Set:` point, and the BPM is smoothed with a
fast EMA so it tracks the knob accurately (±1–2 BPM). For a **physical** Pulse
Sensor, set `USE_SIMULATED_PULSE 0` and wire the sensor to **A0** as described
in `circuit_diagram/`, then run on real Arduino hardware.
