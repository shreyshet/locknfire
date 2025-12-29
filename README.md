
# 🎯 lock-n-fire: Wireless AI Targeting & Automation

**lock-n-fire** is a modular, high-speed targeting framework that utilizes **YOLOv11** to track objects and execute timed trigger commands. By offloading heavy AI processing to a PC and relaying commands wirelessly via **UDP**, the system provides a high-torque, responsive hardware interface for any autonomous project.

![Python Version](img.shields.io)
![YOLO](img.shields.io)
![Hardware](img.shields.io)

---

## 🚀 Key Features

*   **Target Agnostic:** Currently configured for human pose detection (nose), but designed to work with any YOLOv11 model (Face, Hands, or Custom Objects).
*   **Wireless Bridge Architecture:** Communicates via **UDP over Wi-Fi** for low-latency command transmission, eliminating physical tethers.
*   **Intelligent Smoothing:** Uses a `deque`-based moving average filter to ensure smooth actuator movement and eliminate detection "jitter."
*   **Timed Lock-On Mechanism:** Logic-driven trigger system requiring a stable lock (e.g., 2.0s) before sending the **FIRE ('F')** command.
*   **Hardware Isolation:** Uses an ESP32 for networking and an Arduino for high-current actuator control, ensuring system stability during motor surges.

---

## 🛠️ The "Bridge" Protocol

The system utilizes a lightweight ASCII protocol for real-time control: