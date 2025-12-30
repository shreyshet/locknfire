
# 🔥 LockNFire

 Vision‑Guided Autonomous Nerf Turret LockNFire is a computer‑vision‑powered Nerf turret that detects, tracks, and fires at targets using YOLO‑based object detection and a microcontroller‑driven stepper‑motor system. This project blends real‑time vision, embedded control, and custom 3D‑printed hardware into a fully automated mechatronics build.

[![Video Thumbnail](https://github.com/user-attachments/assets/20e8e774-12a3-42f5-8082-aed4a1d9fc35)](https://youtube.com/shorts/vH2sAss3wS0?feature=share)
---

## ✨ Features

- 🎯 Real‑time object detection using YOLO
- 📷 Webcam‑based live tracking
- 🔧 Stepper‑motor turret control
- 🔫 Automated Nerf firing mechanism
- 🖨️ 3D‑printable mechanical parts included in the repo
- 🧩 Modular code structure for easy experimentation and upgrades

---

## 📁 Repository Structure

```bash
locknfire/
│
├── firmware/
│   ├── arduino/               # Arduino-based turret controller
│   └── esp32/                 # ESP32-based turret controller
│   
├── vision/                    # YOLO + OpenCV tracking scripts
│
├── models/                    # 3D-printable STL files
│   ├── Base.stl
│   ├── Top_plate.stl
│   ├── Motor_Gear.stl
│   └── Pin.stl
│
└── README.md                  # You are here
```

---

## 🛠 Hardware Requirements

- USB webcam  
- Arduino Uno **or** ESP32  
- Stepper motor + driver (A4988 / DRV8825)  
- Modified Nerf blaster with trigger actuator  
- 3D‑printed turret components (STLs included)  
- External power supply for motors  

---

## 🧠 Software Requirements

- Python 3.x  
- OpenCV  
- YOLO model (v8/v11 depending on your script)  
- Arduino IDE or PlatformIO  
- Serial communication between Python and microcontroller  

---

## ⚙️ Setup

### 1. Print and Assemble the Turret

All `.stl` files are in the `models/` folder.  
Print them, mount the motors, and attach the Nerf blaster.

### 2. Upload Firmware

Choose your controller:

- `Arduino Code/`  
- `ESP32 Code/`

Upload the `.ino` file using Arduino IDE.

### 3. Run the Vision System

Inside `Python Code/`, run the detection script:

```bash
python Shooter_YOLOV11_Nose.py
```
