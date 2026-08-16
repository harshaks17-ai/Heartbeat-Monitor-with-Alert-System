# Screenshots / Proof Checklist

Capture these for your GitHub proof:

- [ ] Project folder structure (file explorer)
- [ ] Complete Wokwi simulation circuit
- [ ] Circuit diagram (from `circuit_diagram/`)
- [ ] Raw pulse signal output (Serial Plotter / analog values)
- [ ] Normal BPM output (OLED + Serial: ~75, NORMAL, green LED)
- [ ] Low BPM alert (OLED + Serial: ~50, LOW, red LED + buzzer)
- [ ] High BPM alert (OLED + Serial: ~130, HIGH, red LED + buzzer)
- [ ] Green LED state (normal)
- [ ] Red LED state (alert)
- [ ] Buzzer alert state
- [ ] OLED display close-up
- [ ] Serial Monitor output
- [ ] Source code screenshot (e.g., `Config.h` + `BPMCalculator.ino`)
- [ ] GitHub repository preview
- [ ] README preview

Tip: in Wokwi, change `SIM_TARGET_BPM` in `Config.h` to 50 / 75 / 130 to
capture the three alert states reliably.
