import cv2
from ultralytics import YOLO
import firebase_admin
from firebase_admin import credentials, db
import websockets
import asyncio
import json
import base64

print("Starting application...")  # Debug print

# Initialize Firebase
cred = credentials.Certificate("final-project-iot-be6f8-firebase-adminsdk-9pzsg-2de6e32097.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://final-project-iot-be6f8-default-rtdb.firebaseio.com/'
})

print("Firebase initialized")  # Debug print

# Initialize YOLO model
print("Loading YOLO model...")  # Debug print
model = YOLO('yolov8n.pt')
print("YOLO model loaded successfully")  # Debug print


async def video_stream(websocket, path):
    print("New client connected")  # Debug print
    video_capture = cv2.VideoCapture(0)

    if not video_capture.isOpened():
        print("Error: Could not open video capture")  # Debug print
        return

    print("Camera opened successfully")  # Debug print
    ref = db.reference('/sensor_data')

    try:
        while True:
            success, frame = video_capture.read()
            if not success:
                print("Failed to read frame")  # Debug print
                break

            print("Processing frame with YOLO...")  # Debug print
            # Run YOLOv8 inference
            results = model(frame)
            human_detected = False

            # Process results and draw boxes
            for result in results:
                boxes = result.boxes
                for box in boxes:
                    cls = int(box.cls[0])
                    class_name = model.names[cls]
                    print(f"Detected: {class_name}")  # Debug print

                    if class_name == 'person':
                        human_detected = True
                        box_coordinates = box.xyxy[0].tolist()
                        cv2.rectangle(
                            frame,
                            (int(box_coordinates[0]), int(box_coordinates[1])),
                            (int(box_coordinates[2]), int(box_coordinates[3])),
                            (0, 255, 0),
                            2
                        )

            # Update Firebase
            ref.update({'human_detected': human_detected})

            # Convert frame to base64 for sending over WebSocket
            _, buffer = cv2.imencode('.jpg', frame)
            frame_base64 = base64.b64encode(buffer).decode('utf-8')

            try:
                await websocket.send(json.dumps({
                    'image': frame_base64,
                    'human_detected': human_detected
                }))
                print("Frame sent to client")  # Debug print
            except websockets.exceptions.ConnectionClosed:
                print("Client disconnected")  # Debug print
                break

            await asyncio.sleep(0.1)  # Small delay to control frame rate

    finally:
        video_capture.release()
        print("Video capture released")  # Debug print


if __name__ == "__main__":
    print("Starting WebSocket server...")  # Debug print
    start_server = websockets.serve(video_stream, "localhost", 8765)
    print("WebSocket server started on ws://localhost:8765")  # Debug print
    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()