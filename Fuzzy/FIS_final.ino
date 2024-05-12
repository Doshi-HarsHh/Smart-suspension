#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float gyroAngleZ;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;

// Fuzzy Inference System variables and functions
#define FIS_RESOLUSION 101
#define FIS_MIN -1
#define FIS_MAX 1

float fis_input[2];
float fis_output[1];

// Function prototypes
void fis_evaluate();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  calculate_IMU_error();
  delay(20);
}

void loop() {
  // Read accelerometer data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorX;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 - AccErrorY;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
  
  // Read gyroscope data
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorX;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorY;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0 - GyroErrorZ;
  
  // Calculate yaw from the gyroscope data
  // fis_input[0]= 50;//Speed of vehicle
  GyroZ = GyroZ + 0.79; // Correct the gyro output
  gyroAngleZ = gyroAngleZ + GyroZ * elapsedTime;
  fis_input[1] = gyroAngleZ / 90.0; // Normalize yaw to range -1 to 1
  
  // Evaluate the fuzzy inference system
  fis_evaluate();
  
  // Print the output on the serial monitor
  Serial.println(fis_output[0]);

  delay(100); // Adjust as necessary for your application
}

void calculate_IMU_error() {
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    
    // Sum all readings
    AccErrorX = AccErrorX + AccX;
    AccErrorY = AccErrorY + AccY;
    
    c++;
  }
  
  // Divide the sum by 200 to get the average error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
}

// Function to evaluate the fuzzy inference system
void fis_evaluate()
{
    float fuzzyInput0[] = { 0, 0, 0, 0, 0 };
    float fuzzyInput1[] = { 0, 0, 0, 0, 0, 0, 0 };
    float* fuzzyInput[num_inputs] = { fuzzyInput0, fuzzyInput1, };
    float fuzzyOutput0[] = { 0, 0, 0, 0, 0 };
    float* fuzzyOutput[num_outputs] = { fuzzyOutput0, };
    float fuzzyRules[num_rules] = { 0 };
    float fuzzyFires[num_rules] = { 0 };
    float* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
    float sW = 0;

    // Transforming input to fuzzy Input
    int i, j, r, o;
    for (i = 0; i < num_inputs; ++i)
    {
        for (j = 0; j < input_MF_count[i]; ++j)
        {
            fuzzyInput[i][j] =
                (triMF_[triMF_I[i][j]])(fis_input[i], triMF_ICoeff[i][j]);
        }
    }

    int index = 0;
    for (r = 0; r < num_rules; ++r)
    {
        if (rule_types[r] == 1)
        {
            fuzzyFires[r] = FIS_MAX;
            for (i = 0; i < num_inputs; ++i)
            {
                index = rule_num[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_min(fuzzyFires[r], 1);
            }
        }
        else
        {
            fuzzyFires[r] = FIS_MIN;
            for (i = 0; i < num_inputs; ++i)
            {
                index = rule_num[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_max(fuzzyFires[r], 0);
            }
        }

        fuzzyFires[r] = rule_weights[r] * fuzzyFires[r];
        sW += fuzzyFires[r];
    }

    if (sW == 0)
    {
        for (o = 0; o < num_outputs; ++o)
        {
           fis_output[o] = ((output_range_max[o] + output_range_min[o]) / 2);
        }
    }
    else
    {
        for (o = 0; o < num_outputs; ++o)
        {
            float sWI = 0.0;
            for (j = 0; j < output_MF_count[o]; ++j)
            {
                fuzzyOutput[o][j] = triMF_OCoeff[o][j][num_inputs];
                for (i = 0; i < num_inputs; ++i)
                {
                    fuzzyOutput[o][j] += fis_input[i] * triMF_OCoeff[o][j][i];
                }
            }

            for (r = 0; r < num_rules; ++r)
            {
                index = rule_output[r][o] - 1;
                sWI += fuzzyFires[r] * fuzzyOutput[o][index];
            }

           fis_output[o] = sWI / sW;
        }
    }
}

