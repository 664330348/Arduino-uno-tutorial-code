# 🔌 Arduino Wiring Guide

This document records the wiring connections.

---

# 📟 LCD1602 (16-pin, 4-bit mode)

| LCD Pin | Name      | Description          | Arduino Connection       |
|---------|-----------|----------------------|--------------------------|
| 1       | VSS       | GND                  | GND                      |
| 2       | VDD       | +5V                  | 5V                       |
| 3       | VO        | Contrast             | 10K potentiometer middle |
| 4       | RS        | Register Select      | D12                      |
| 5       | RW        | Read/Write           | GND                      |
| 6       | E         | Enable               | D11                      |
| 7–10    | D0–D3     | Not used             | —                        |
| 11      | D4        | Data                 | D7                       |
| 12      | D5        | Data                 | D6                       |
| 13      | D6        | Data                 | D5                       |
| 14      | D7        | Data                 | D4                       |
| 15      | A/LED+    | Backlight +          | 5V                       |
| 16      | K/LED-    | Backlight -          | GND                      |

---

# 🌡️ DHT11 Temperature & Humidity Sensor

| Pin | Description | Arduino |
|-----|------------|--------|
| DATA| Signal     | D2     |
| VCC | Power      | 5V     |
| GND | Ground     | GND    |

---

# 🔌 Relay Module (5V)

| Pin    | Description | Arduino |
|--------|-------------|---------|
| VCC    | Power       | 5V      |
| GND    | Ground      | GND     |
| IN1/2  | Control     | D8      |

### Load Side
| Terminal | Description     |
|----------|-----------------|
| COM      | Common          |
| NO       | Normally Open   |
| NC       | Normally Closed |

---

# ⌨️ 4x4 Keypad

| Keypad Pin | Arduino Connection |
|------------|-------------------|
| R1         | D2                |
| R2         | D3                |
| R3         | D4                |
| R4         | D5                |
| C1         | D6                |
| C2         | D7                |
| C3         | D8                |
| C4         | D9                |

> ⚠️ Note: Pin order may vary by module. Adjust in code if keys are mismatched.

---

# 🎮 Joystick Module (5V)

| Pin    | Description        | Arduino UNO |
|--------|--------------------|-------------|
| GND    | Ground             | GND         |
| VCC    | Power              | 5V          |
| VRX    | X-axis analog out  | A5          |
| VRY    | Y-axis analog out  | A4          |
| SW     | Button (switch)    | D2          |

### Notes
- VRX controls left/right movement (X-axis)
- VRY controls up/down movement (Y-axis)
- SW is LOW when pressed, HIGH when released (usually uses INPUT_PULLUP)

---

# ⚠️ Notes

- Avoid using D0 and D1 (Serial pins) if using Serial Monitor
- Double-check shared pins between modules to avoid conflicts
- Always connect GND together across all components

---