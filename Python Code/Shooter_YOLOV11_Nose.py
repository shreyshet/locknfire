from ultralytics import YOLO
import cv2
import socket
from collections import deque
import time

# --- CONFIGURATION ---
ESP_IP = "192.168.4.172"  # REPLACE with the IP printed in Arduino Serial Monitor
ESP_PORT = 4210
SMOOTH_WINDOW = 5  # Number of frames to average
TRIGGER_THRESHOLD = 15 # "Center" zone in pixels
LOCK_REQUIRED_TIME = 2.0 # Seconds to hold lock before firing
# ---------------------

# Timer variables
lock_start_time = None
last_fire_time = 0
fire_cooldown = 2.0 # Wait 2 seconds before being allowed to fire again

# Setup UDP Socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
x_buffer = deque(maxlen=SMOOTH_WINDOW)
model = YOLO("yolo11n-pose.pt")
cap = cv2.VideoCapture(2)
frame_center_x = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)) // 2

last_send_time = 0
send_interval = 0.05  # 20 messages per second (Fast updates)

def get_smoothed_x(new_x):
    x_buffer.append(new_x)
    return sum(x_buffer) / len(x_buffer)

while True:
    ret, frame = cap.read()
    if not ret: break

    results = model(frame, verbose=False)[0]

    if results.keypoints is not None and len(results.keypoints.xy) > 0:
        # Access the first person's nose (Index 0)
        # xy[person_index][keypoint_index]
        # Access the first person detected
        person_kpts = results.keypoints.xy[0]
        if len(person_kpts) > 0:
            nose_coords = results.keypoints.xy[0][0]

            raw_x = int(nose_coords[0])
            raw_y = int(nose_coords[1])

            if raw_x > 0:
                smooth_x = get_smoothed_x(raw_x)
                diff = smooth_x - frame_center_x

                # 2. Command Logic
                is_centered = abs(diff) < TRIGGER_THRESHOLD
                elapsed = 0.0

                if is_centered:
                    if lock_start_time is None:
                        lock_start_time = time.time()

                    elapsed = time.time() - lock_start_time
                    if elapsed >= LOCK_REQUIRED_TIME:
                        msg = "FIRE!"  # String for terminal
                        cmd = "F"  # Character for ESP32
                        lock_start_time = None
                    else:
                        msg = "LOCKING"
                        cmd = "S"
                else:
                    lock_start_time = None
                    elapsed = 0.0
                    if diff < -60:
                        msg = "MOVE LEFT (FAST)"; cmd = "L"
                    elif diff < -15:
                        msg = "MOVE LEFT (SLOW)"; cmd = "l"
                    elif diff > 60:
                        msg = "MOVE RIGHT (FAST)"; cmd = "R"
                    elif diff > 15:
                        msg = "MOVE RIGHT (SLOW)"; cmd = "r"
                    else:
                        msg = "IDLE"; cmd = "S"

                # 3. THE REPORTING MESSAGE
                # \r returns the cursor to the start of the line for a clean overwrite
                timer_display = f"{elapsed:3.1f}s" if is_centered else "----"

                report = (
                    f"| STATUS: {msg:<18} "  # Left-aligned status text
                    f"| CMD SENT: {cmd} "  # The actual character going to ESP32
                    f"| TIMER: {timer_display} "
                    f"| DIFF: {int(diff):4}px | \n"
                )
                print("COME ON!!!!\n")

                print(report)

                sock.sendto(cmd.encode(), (ESP_IP, ESP_PORT))

                # --- VISUAL FEEDBACK ---
                color = (0, 255, 0) if not is_centered else (0, 165, 255)  # Green vs Orange
                if is_centered and lock_start_time:
                    # Draw a progress bar for the 5-second lock
                    progress = int((time.time() - lock_start_time) / LOCK_REQUIRED_TIME * 100)
                    cv2.putText(frame, f"LOCKING: {progress}%", (50, 50),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 165, 255), 2)

                cv2.circle(frame, (int(smooth_x), 200), 10, color, -1)

    cv2.imshow("Smoothed Tracking", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'): break

cap.release()
cv2.destroyAllWindows()