You can see the video file attached above for full setup process (you need to download the video)

# ⚡ Smart Energy Meter (IoT-Based)

A smart IoT energy meter that helps users monitor electricity consumption in real-time through the **Blynk IoT app**. The system tracks usage, estimates costs, detects overloads, and can notify users of outages.

## 🚀 Features

- 🔌 Real-time energy monitoring via Blynk mobile app
- 🕒 Data logging with timestamps
- ⚠️ Overload detection and alerting
- 📡 Wireless data transmission using ESP8266 / ESP32
- 🔍 Voltage and current sensing
- ☁️ Cloud dashboard and control with Blynk IoT
- 💡 Power outage notifications
- 💰 Optional recharge/prepaid mode
- 🖥️ On-device OLED display for local stats

## 🧰 Hardware Used

- ESP8266 (NodeMCU) or ESP32
- Voltage Sensor (e.g., ZMPT101B)
- Current Sensor (e.g., ACS712 or SCT-013-030)
- OLED Display (0.96" / 1.3")
- Relay Module (for load cutoff)
- Power Supply (5V regulated)
- Misc: resistors, capacitors, jumper wires
- *(Optional)* SD Card Module for offline storage

## 🧑‍💻 Software & Tools

- [Arduino IDE](https://www.arduino.cc/en/software) – for programming the ESP board
- [Blynk IoT Platform](https://blynk.io/) – for mobile dashboard and control
- *(Optional)* Firebase / MQTT – for advanced cloud logging
- Libraries: Blynk, Adafruit_SSD1306, ACS712, ZMPT101B, etc.


## 📲 Using Blynk

1. Download the **Blynk IoT** app (iOS/Android).
2. Create a new template and add virtual widgets (e.g., gauges, LED indicators).
3. Link virtual pins to usage values from the code.
4. Use Blynk Auth Token in your `.ino` code for connection.

## 🔔 Alerts & Control

- Automatic alerts on overuse via Blynk notifications.
- Manual or auto cutoff control using the relay module.
- Optional prepaid or credit-based cutoff system.

## 🛠 Setup Instructions

1. **Connect sensors and load** as per your circuit.
2. **Upload the code** via Arduino IDE (ESP board selected).
3. **Enter your Wi-Fi and Blynk Auth Token** in the code.
4. **Open Blynk app** to view real-time usage and controls.

## 💡 Future Improvements

- Monthly report generation
- Solar integration and bi-directional energy tracking
- Energy-saving tips based on consumption trends
- Multi-user access and admin roles



---

💡 Built to make electricity monitoring smarter and accessible for everyone.




Circuit Diagram
![Image](https://github.com/user-attachments/assets/a33c5e71-d7ab-4eb1-ae65-f040a0357fba)
