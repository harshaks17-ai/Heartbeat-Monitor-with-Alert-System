💻 Excited to Share My New Embedded Systems Project! 🚀
❤️ Heartbeat Monitor with Alert System

Heartbeat Monitor with Alert System is an embedded systems project that measures heart rate and raises threshold alerts, using a Pulse Sensor, an SSD1306 OLED, a Buzzer, and LEDs with an Arduino UNO (simulated in Wokwi).

It continuously samples the pulse waveform, detects individual heartbeats, computes Beats Per Minute (BPM), and compares it against configurable educational thresholds. It shows live BPM on an OLED and raises a buzzer + red-LED alert when rate is low/high; green LED = normal. A potentiometer controls heart rate for demos.

How It Works:
The Pulse Sensor outputs an analog voltage that rises with each heartbeat. The microcontroller reads it through ADC, tracks a moving baseline and adaptive peak threshold to detect beats, and converts the inter-beat interval into BPM using exponential smoothing for stable readings. Threshold logic drives the alerts, while a potentiometer sets the target BPM for simulation.

✨ Key Features
• Real-Time BPM Calculation (Pulse Detection + Smoothing)
• Threshold-Based Alert System (Low / High Heart Rate)
• Buzzer + Red LED Alert Alarm
• Green LED Normal-Range Indicator
• SSD1306 OLED Live BPM & Status Display
• Potentiometer Heart-Rate Regulator (Live Control)
• Simulated Pulse Mode (No Hardware Needed)
• Modular Embedded C++ Architecture
• Fully Simulated in Wokwi

🛠️ Tech Stack
Arduino UNO | Pulse Sensor | SSD1306 OLED | Buzzer | LEDs | Embedded C++ | GPIO & ADC | I2C | Wokwi | Git | GitHub

📚 What I Learned
This project strengthened my understanding of analog sensor interfacing, ADC, signal conditioning, peak detection, timer-based beat timing, exponential smoothing, threshold logic, and I/O control, plus modular embedded C++ design and simulation-based testing.

Grateful for this opportunity — it strengthened my technical knowledge and problem-solving skills.
Thanks to my mentor @umeshcnyadav for the guidance and support throughout!

🔗 GitHub Repository:
https://github.com/harshaks17-ai/Heartbeat-Monitor-with-Alert-System

#EmbeddedSystems #Arduino #HeartbeatMonitor #PulseSensor #EmbeddedC #Biomedical #Wokwi #IoT
