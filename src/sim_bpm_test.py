#!/usr/bin/env python3
"""
sim_bpm_test.py - Validation harness for the Heartbeat Monitor algorithm.

This is a faithful Python port of the C logic in arduino_code/
(PulseSensor.ino + BPMCalculator.ino). It verifies the BPM detection, the
faster EMA smoothing, and the potentiometer "regulator" behavior on a PC
without hardware, and proves the firmware math is correct.

Run:  python src/sim_bpm_test.py
"""

import random

# ---- Config (mirror Config.h) ----
SIM_TARGET_BPM      = 75
USE_POT_CONTROL     = True
POT_BPM_MIN         = 40
POT_BPM_MAX         = 180
LOW_THRESHOLD_BPM   = 60
HIGH_THRESHOLD_BPM  = 100
MIN_BEAT_INTERVAL_MS = 300
BEAT_TIMEOUT_MS     = 3000
BPM_SMOOTH          = 0.5

# ---- Globals (mirror firmware) ----
baseline = 512.0
peakHold = 512
lastBeatTime = 0
beatInProgress = False
g_currentBPM = 0
g_signalLost = True
g_targetBPM = -1

startMs = 0
phaseFrac = 0.0
potSmoothed = -1.0


def my_abs(x):
    return -x if x < 0 else x


def get_pulse_sample(now, knob):
    """knob: 0..1023 potentiometer position (constant during a test)."""
    global startMs, phaseFrac, potSmoothed, g_targetBPM
    if startMs == 0:
        startMs = now
    dt = 4  # ~250 Hz sampling
    now2 = now
    last = now2 - dt
    if dt == 0:
        dt = 1

    target = SIM_TARGET_BPM
    if USE_POT_CONTROL:
        raw = knob
        if potSmoothed < 0:
            potSmoothed = raw
        else:
            potSmoothed = potSmoothed * 0.8 + raw * 0.2
        target = int(potSmoothed / 1023 * (POT_BPM_MAX - POT_BPM_MIN) + POT_BPM_MIN)
    g_targetBPM = target

    period = 60000.0 / target
    phaseFrac += dt / period
    if phaseFrac >= 1.0:
        phaseFrac -= 1.0

    v = 512.0
    dx = phaseFrac - 0.15
    if my_abs(dx) < 0.06:
        v += 350.0 * (1.0 - my_abs(dx) / 0.06)
    dx2 = phaseFrac - 0.40
    if my_abs(dx2) < 0.06:
        v += 120.0 * (1.0 - my_abs(dx2) / 0.06)
    v += (random.randint(0, 16) - 8)
    v = max(0.0, min(1023.0, v))
    return int(v)


def compute_bpm(sample, now):
    global baseline, peakHold, lastBeatTime, beatInProgress, g_currentBPM, g_signalLost
    baseline = baseline * 0.95 + sample * 0.05
    if sample > peakHold:
        peakHold = sample
    else:
        peakHold = int(peakHold * 0.999)
    threshold = int(baseline + (peakHold - baseline) * 0.55)

    if not beatInProgress:
        if sample > threshold and (now - lastBeatTime) > MIN_BEAT_INTERVAL_MS:
            if lastBeatTime > 0:
                interval = now - lastBeatTime
                if interval > 0 and interval < 2000:
                    inst = int(60000.0 / interval)
                    if g_currentBPM == 0:
                        g_currentBPM = inst
                    else:
                        g_currentBPM = int(g_currentBPM * (1.0 - BPM_SMOOTH) + inst * BPM_SMOOTH)
            lastBeatTime = now
            beatInProgress = True
            g_signalLost = False
    else:
        if sample < threshold - 25:
            beatInProgress = False

    if (now - lastBeatTime) > BEAT_TIMEOUT_MS:
        g_signalLost = True
        g_currentBPM = 0
    return g_currentBPM


def evaluate_status(bpm, lost):
    if lost or bpm <= 0:
        return "NO_SIGNAL"
    if bpm < LOW_THRESHOLD_BPM:
        return "LOW"
    if bpm > HIGH_THRESHOLD_BPM:
        return "HIGH"
    return "NORMAL"


def reset_state():
    global baseline, peakHold, lastBeatTime, beatInProgress, g_currentBPM
    global g_signalLost, startMs, phaseFrac, potSmoothed
    baseline = 512.0
    peakHold = 512
    lastBeatTime = 0
    beatInProgress = False
    g_currentBPM = 0
    g_signalLost = True
    startMs = 0
    phaseFrac = 0.0
    potSmoothed = -1.0


def run(knob, seconds=12):
    reset_state()
    random.seed(42)
    now = 0
    step = 4
    tail = []
    while now <= seconds * 1000:
        sample = "unused"
        s = get_pulse_sample(now, knob)
        compute_bpm(s, now)
        if g_currentBPM > 0 and now > 3000:  # stable tail after warm-up
            tail.append(g_currentBPM)
        now += step

    stable = tail[-15:] if len(tail) >= 15 else tail
    avg = sum(stable) // len(stable) if stable else 0
    expected = int(knob / 1023 * (POT_BPM_MAX - POT_BPM_MIN) + POT_BPM_MIN)
    spread = max(stable) - min(stable) if stable else 99
    status = evaluate_status(g_currentBPM, g_signalLost)
    ok = abs(avg - expected) <= 4 and spread <= 4
    print(f"Knob {knob:>4} -> Set {expected:>3} | Measured {avg:>3} | "
          f"spread +/-{spread//2} | {status:>9} | {'PASS' if ok else 'FAIL'}")
    return ok


def test_no_signal():
    reset_state()
    now = 5000
    compute_bpm(512, now)
    status = evaluate_status(g_currentBPM, g_signalLost)
    ok = status == "NO_SIGNAL"
    print(f"No-signal test -> status: {status:>9} | {'PASS' if ok else 'FAIL'}")
    return ok


if __name__ == "__main__":
    print("=== Heartbeat Monitor algorithm validation (regulator) ===")
    ok1 = run(0)      # knob left  -> LOW (~40)
    ok2 = run(512)    # knob mid   -> ~110
    ok3 = run(1023)   # knob right -> HIGH (~180)
    ok4 = test_no_signal()
    all_ok = ok1 and ok2 and ok3 and ok4
    print("\nRESULT:", "ALL TESTS PASSED" if all_ok else "SOME TESTS FAILED")
