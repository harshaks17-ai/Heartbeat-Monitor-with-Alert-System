# Circuit Diagram & Wiring

## Arduino UNO wiring

```
                 +-----------------------+
                 |     Arduino UNO       |
                 |                       |
            5V --+--[VCC] Pulse Sensor   |
           GND --+--[GND] Pulse Sensor   |
            A0 --+--[SIG] Pulse Sensor   |
                 |                       |
            5V --+--[VCC] OLED           |
           GND --+--[GND] OLED           |
            A4 --+--[SDA] OLED           |
            A5 --+--[SCL] OLED           |
                 |                       |
             D7 --+--[220R]--[+] GREEN --+-- GND
             D6 --+--[220R]--[+] RED   --+-- GND
             D8 --+--[+] BUZZER --+-- GND
                 +-----------------------+

   Potentiometer (Wokwi "regulator"):
                 +-----------------------+
            5V --+--[VCC] POT
           GND --+--[GND] POT
            A1 --+--[SIG] POT
                 +-----------------------+
```

## Pin map

| Arduino | Component | Notes |
|---|---|---|
| A0 | Pulse Sensor SIG | analog input (real sensor) |
| A1 | Potentiometer SIG | sim regulator (Wokwi) |
| A4 | OLED SDA | I2C |
| A5 | OLED SCL | I2C |
| D6 | Red LED (via 220Ω) | alert |
| D7 | Green LED (via 220Ω) | normal |
| D8 | Buzzer (+) | alert |

> ESP32 variant: use GPIO34 for the sensor (ADC1), and any GPIO for
> buzzer/LEDs; OLED on default I2C (GPIO21 SDA / GPIO22 SCL).
