# 🛰️ Real-Time Radar Obstacle Detection System

A real-time 180° radar visual system built on the **STM32F429I-DISC1** Discovery kit using an **HC-SR04** ultrasonic sensor, **SG90 Servo**, **Active/Passive Buzzer**, and **TouchGFX** GUI framework.

![STM32F429](https://img.shields.io/badge/MCU-STM32F429ZI-blue.svg)
![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green.svg)
![TouchGFX](https://img.shields.io/badge/GUI-TouchGFX-brightgreen.svg)
![Language](https://img.shields.io/badge/Language-C%20%2F%20C%2B%2B-orange.svg)

---

## 📸 Overview & Features

* **180° Radar Sweep:** Dynamic sweep line synced with SG90 Servo PWM output.
* **1D Kalman Filtering:** Smooths out raw ultrasonic noise and prevents position jumps on distant target detection.
* **Persistent Mapping:** Custom `RadarCanvas` C++ widget renders all detected obstacle points (`radarMap`) throughout a full $180^\circ$ sweep cycle before resetting.
* **Proximity Alert:** Integrated GPIO-driven active/passive buzzer providing real-time audio warnings when obstacles are within the hazard zone ($<20\text{ cm}$).
* **TouchGFX MVP Architecture:** Clean separation between hardware driver tasks (Model/FreeRTOS) and GUI presentation (View/Presenter).

---

## 🛠️ Hardware Requirements

* **Development Board:** STM32F429I-DISC1 (2.4" QVGA TFT LCD)
* **Ultrasonic Sensor:** HC-SR04 (5V supply)
* **Servo Motor:** SG90 Servo
* **Audio Warning:** Active/Passive 2-pin Buzzer + NPN Transistor (S8050/2N2222) + $1\text{k}\Omega$ Resistor
* **Power Supply:** External 5V DC power supply for Servo and HC-SR04

---

## 🔌 Pin Mapping

| Peripheral | STM32 Pin | Function / Description |
| :--- | :--- | :--- |
| **HC-SR04 Trigger** | `PB3` | Output (10µs trigger pulse) |
| **HC-SR04 Echo** | `PB4` | Input (Timer input capture / DWT time calculation) |
| **SG90 Servo** | `PA0` (TIM5_CH1) | PWM Output (50Hz, 1ms–2ms pulse) |
| **Buzzer** | `PB7` | GPIO Output (NPN Transistor Base control) |

---

## 📐 System Architecture

```text
+-----------------------+      +-------------------------+      +------------------------+
| HC-SR04 & SG90 Servo  | ---> |   FreeRTOS Sensor Task  | ---> |   1D Kalman Filter     |
+-----------------------+      +-------------------------+      +------------------------+
                                                                            |
                                                                            v
+-----------------------+      +-------------------------+      +------------------------+
| Active/Passive Buzzer | <--- | TouchGFX Presenter/View | <--- |   `radarMap[181]`      |
+-----------------------+      +-------------------------+      +------------------------+

<img width="3072" height="4080" alt="953" src="https://github.com/user-attachments/assets/2bd0246c-10a2-4151-a2d4-7b2723176c50" />

youtube: https://www.youtube.com/shorts/u_6eYctj4yQ





