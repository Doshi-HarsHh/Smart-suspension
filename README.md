# Smart-Suspension
Welcome to Smart-Suspension, an innovative suspension system designed to enhance both safety and comfort in four-wheeled vehicles. This project combines fuzzy logic and deep learning techniques to create an intelligent suspension system that adapts to road conditions in real-time.

# **Key Features**:

**Fuzzy Logic Control**: The system employs a custom-developed Sugeno Type 2 Fuzzy Inference System (FIS) implemented on Arduino with MATLAB's support. This FIS considers two primary inputs: vehicle speed and the severity of encountered bumps or potholes. Leveraging the Sugeno Type 2 FIS enables accurate decision-making in the presence of uncertainty and imprecision.


**Sensor Integration**: Utilizing the MPU6050 sensor, the system detects Z-axis motion to identify bumps or potholes during vehicle operation.


**Dynamic Suspension Adjustment**: If the vehicle experiences instability exceeding a predefined threshold due to road irregularities, the FIS triggers a servo motor. This motor dynamically adjusts the suspension stiffness based on real-time inputs, ensuring optimal vehicle stability and ride comfort.


**Deep Learning Enhancement**: To address the system's reactive nature post-instability, an instance segmentation technique powered by deep learning is integrated. This technique, deployed on a Raspberry Pi 5 single-board computer with a camera, detects road obstacles ahead, allowing proactive suspension adjustments for a smoother and safer ride.

**Yolo V5 Model**: The instance segmentation model is trained on a custom dataset comprising images of potholes and bumps. Leveraging the efficiency and accuracy of Yolo V5, a state-of-the-art deep learning model, ensures real-time detection performance.
