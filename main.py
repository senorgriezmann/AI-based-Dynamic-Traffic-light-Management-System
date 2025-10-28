import cv2
import serial
from ultralytics import YOLO

# Load the YOLOv8 model pre-trained on the COCO dataset
model = YOLO('yolo11n.pt')  # Replace with the correct path to your YOLO weights file

# Define the classes for vehicles
vehicle_classes = ['car', 'truck', 'bus', 'motorcycle']

# Input image paths
input_images = [
    'lane1.jpg',  # Replace with your first input image path
    'lane2.jpg',  # Replace with your second input image path
    'lane3.jpg',  # Replace with your third input image path
    'lane4.jpg',  # Replace with your fourth input image path
]

# Number of lanes (assuming consistent lane width across images)
num_lanes = 4

# Initialize lane vehicle counts
total_lane_counts = [0] * num_lanes

# Configure the serial port
serial_port = 'COM1'  # Replace with the COM port connected to the Arduino
baud_rate = 9600
try:
    ser = serial.Serial(serial_port, baud_rate, timeout=2)
except serial.SerialException as e:
    print(f"Error: Unable to open serial port {serial_port}. {e}")
    exit(1)

# Function to assign a vehicle to a lane
def assign_lane(x_center, lane_width, num_lanes):
    lane = int(x_center // lane_width)
    return min(lane, num_lanes - 1)  # Ensure lane index doesn't exceed total lanes

# Process each image
for img_path in input_images:
    # Load the image
    image = cv2.imread(img_path)
    if image is None:
        print(f"Error: Unable to load image {img_path}. Skipping...")
        continue

    # Get image dimensions
    height, width, _ = image.shape
    lane_width = width / num_lanes

    # Run inference
    results = model(image)

    # Temporary lane counts for the current image
    lane_counts = [0] * num_lanes

    # Iterate through detections
    for result in results[0].boxes:
        # Extract class ID and confidence
        class_id = int(result.cls[0])
        confidence = result.conf[0]

        # Check if the detected object is a vehicle
        if model.names[class_id] in vehicle_classes:
            # Get bounding box coordinates
            x1, y1, x2, y2 = map(int, result.xyxy[0])
            x_center = (x1 + x2) / 2

            # Assign the vehicle to a lane
            lane = assign_lane(x_center, lane_width, num_lanes)
            lane_counts[lane] += 1

            # Draw bounding box and label on the image
            cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(image, model.names[class_id], (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # Update total lane counts
    for i in range(num_lanes):
        total_lane_counts[i] += lane_counts[i]

    # Display the image with detections (optional)
    cv2.imshow('Image', image)
    cv2.waitKey(0)  # Wait for a key press to close the window

# Send the final vehicle counts to the Arduino via serial
density_string = ' '.join(map(str, total_lane_counts)) + '\n'
try:
    ser.write(density_string.encode())
    print(f"Data sent to Arduino: {density_string.strip()}")
except serial.SerialException as e:
    print(f"Error: Unable to send data to Arduino. {e}")

# Print the final vehicle counts for verification
print("\nFinal Vehicle Counts Sent to Arduino:")
for i, count in enumerate(total_lane_counts):
    print(f"Lane {i + 1}: {count} vehicles")

print(f"Data sent to Arduino: {density_string.strip()}")

# Close the serial port
ser.close()

# Close all OpenCV windows
cv2.destroyAllWindows()
