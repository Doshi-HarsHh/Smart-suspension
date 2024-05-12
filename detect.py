from ultralytics import YOLO
import cv2
import numpy as np

# Load the model
model = YOLO("besttt.pt")
class_names = model.names

# Open the video file
cap = cv2.VideoCapture('pot2.mp4')

# Define the codec and create a VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # Use a suitable codec for your video format
out = cv2.VideoWriter('output.mp4', fourcc, 30.0, (640, 640))  # Adjust frame rate and dimensions

# Define the region of interest for the left tire
left_tire_roi = [(0, 640), (190, 640), (200, 200), (0, 200)]

# Define the region of interest for the right tire
right_tire_roi = [(450, 640), (640, 640), (640, 200), (450, 200)]

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.resize(frame, (640, 640))
    
    # Draw translucent ROIs on the frame
    left_mask = np.zeros_like(frame)
    right_mask = np.zeros_like(frame)
    cv2.fillPoly(left_mask, [np.array(left_tire_roi)], (0, 0, 255))
    cv2.fillPoly(right_mask, [np.array(right_tire_roi)], (0, 0, 255))
    frame = cv2.addWeighted(frame, 1, left_mask, 0.4, 0)
    frame = cv2.addWeighted(frame, 1, right_mask, 0.4, 0)
    
    results = model.predict(frame)

    for r in results:
        boxes = r.boxes
        masks = r.masks

        if masks is not None:
            masks = masks.data.cpu()
            for seg, box in zip(masks.data.cpu().numpy(), boxes):
                seg = cv2.resize(seg, (frame.shape[1], frame.shape[0]))
                contours, _ = cv2.findContours((seg).astype(np.uint8), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                for contour in contours:
                    d = int(box.cls)
                    c = class_names[d]
                    x, y, x1, y1 = cv2.boundingRect(contour)
                    
                    # Check if contour is within left tire region
                    if cv2.pointPolygonTest(np.array(left_tire_roi), (x, y), False) >= 0:
                        cv2.polylines(frame, [contour], True, color=(0, 0, 255), thickness=1)
                        cv2.putText(frame, c + ' (Left)', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)
                    
                    # Check if contour is within right tire region
                    if cv2.pointPolygonTest(np.array(right_tire_roi), (x, y), False) >= 0:
                        cv2.polylines(frame, [contour], True, color=(0, 0, 255), thickness=1)
                        cv2.putText(frame, c + ' (Right)', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)

    # Write the processed frame to the output video
    out.write(frame)

    # Display the processed frame
    cv2.imshow('Video', frame)
    
    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
out.release()
cv2.destroyAllWindows()
