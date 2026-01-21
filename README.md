# Ultrasonic Radar and Display System (Tiva C)

**Language:** C  
**Microcontroller:** TI Tiva C Series TM4C123GH6PM (ARM Cortex-M4)  
**Hardware:** HC-SR04 Ultrasonic Sensor, SSD1306 OLED (or LCD), Servo Motor  
**Tools:** Keil uVision / Code Composer Studio (CCS), TivaWare SDK

---

## 1. Project Overview
This project is an embedded real-time radar system that utilizes an ultrasonic sensor to detect objects within a 180-degree field of view. By interfacing a servo motor with an ultrasonic sensor, the system performs a continuous sweep, processing distance data and visualizing it on a display to simulate a traditional radar interface.

---

## 2. Key Features

### Precise Distance Measurement
* **Time-of-Flight Logic:** Implemented high-precision timing using **Input Capture Timers** to measure the pulse width of the HC-SR04 echo signal with microsecond accuracy.
* **Non-Blocking I/O:** Utilised **Interrupt Service Routines (ISRs)** for sensor triggering and echo detection, ensuring the CPU remains available for display updates and data processing.
* **Signal Filtering:** Applied basic digital filtering (moving average) to stabilize sensor readings and reduce noise caused by sonic reflections.



### Real-Time Radar Visualisation
* **Dynamic Sweep Display:** Renders a 180-degree "sweep" animation on the display, mapping polar coordinates (angle/distance) to Cartesian pixels.

### Embedded Optimisation
* **Peripheral Integration:** Configured and managed multiple hardware peripherals including **GPIO**, **GPTM (General Purpose Timer)**, **PWM** (for servo control), and **I2C/SPI** (for display communication).
* **Low Power Design:** Optimised the main execution loop to minimise CPU cycles, utilising a timer-driven state machine rather than busy-waiting.

---
