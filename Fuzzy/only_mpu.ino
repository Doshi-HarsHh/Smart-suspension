#include <Wire.h>
#include <Servo.h>

const int MPU = 0x68; // MPU6050 I2C address
float AccZ, GyroZ; // Z-axis accelerometer and gyroscope readings
float AccErrorZ, GyroErrorZ; // Error values for Z-axis
float elapsedTime, currentTime, previousTime; // Time variables
int c = 0;

Servo servo;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  calculate_IMU_error();
  
  servo.attach(9); // Attach servo to pin 9
}

void loop() {
  // Read accelerometer data for Z-axis
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorZ;

  // Read gyroscope data for Z-axis
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;
  Wire.beginTransmission(MPU);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true);
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorZ;

  // Translate accelerometer and gyroscope readings into text descriptions
  String accelerationStatus = "";
  String gyroscopicStatus = "";

  if (AccZ > 1.0) {
    accelerationStatus = "Hard acceleration upwards";
    servo.write(180); // Turn servo right
  } else if (AccZ < -1.0) {
    accelerationStatus = "Hard deceleration or acceleration downwards";
    servo.write(0); // Turn servo left
  } else {
    accelerationStatus = "No significant acceleration";
    servo.write(90); // Center servo
  }

  if (GyroZ > 50.0) {
    gyroscopicStatus = "Fast rotation clockwise";
  } else if (GyroZ < -50.0) {
    gyroscopicStatus = "Fast rotation counterclockwise";
  } else {
    gyroscopicStatus = "No significant rotation";
  }

  // Print the text descriptions on the serial monitor
  Serial.print("Acceleration: ");
  Serial.println(accelerationStatus);
  Serial.print("Gyroscopic movement: ");
  Serial.println(gyroscopicStatus);

  // Delay for stability
  delay(1000);
}
void calculate_IMU_error() {
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 2, true);
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

    // Sum all readings
    AccErrorZ += AccZ;

    c++;
  }

  // Divide the sum by 200 to get the average error value
  AccErrorZ /= 200;
  c = 0;
}
