# 🤖 Tendon-Driven Ganesha Trunk Controller

This project controls a **tendon-driven robotic Ganesha idol's trunk** using 6 motors and encoders, powered by an **Arduino Mega 2560**. The trunk mimics natural, elephant-like motion through hardcoded sequences, and movements are precisely regulated using PID control.

---

## 🎯 Features

- 🦴 **6 DOF tendon-driven trunk** movement
- 🎯 **Angle-based PID control** using quadrature encoders
- 🔄 **Predefined motion sequence**
- 🧠 Controlled via **Arduino Mega**
- 📊 Real-time serial feedback of each joint angle

---

## ⚙️ System Overview

| Component      | Description                                  |
|----------------|----------------------------------------------|
| MCU            | Arduino Mega 2560                            |
| Actuators      | 6 DC motors (for tendon pulling)             |
| Sensors        | 6 quadrature encoders                        |
| Motion Control | PID (Proportional, Integral, Derivative)     |
| Power Supply   | External motor power + logic power separation|

---

## 🧠 Motion Sequence

1. M2 → 68°
2. M1 → 45°
3. Delay 2s
4. M6 → 67°
5. Delay 5s → M6 → 134° (reverse)
6. M4 → 84°
7. Delay 5s → M4 → 168° (reverse)
8. M1 → 90° (reverse)
9. M2 → 136° (reverse)
10. Stop all motors

---

## 📌 Pin Mapping

| Motor | PWM Pin | DIR Pin | Encoder Pin |
|-------|---------|---------|-------------|
| M1    | 13      | 22      | 2           |
| M2    | 12      | 24      | 3           |
| M3    | 11      | 26      | 18          |
| M4    | 10      | 28      | 19          |
| M5    | 9       | 30      | 20          |
| M6    | 8       | 32      | 21          |

---

## 📂 File Structure

```plaintext
tendon-driven-ganesha-trunk/
├── ckt diagram/
│   └── trunk_schematic.jpeg        # Circuit/wiring diagram for the trunk control
├── hardcode_trunk_movement.ino     # Main sketch with PID control and motion sequence
├── angle_measure.ino               # Encoder-based angle measurement test
└── README.md                       # Project documentation (this file)
