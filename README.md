# Smart-suspension System
**It's a semi-active suspension system actuated by servo motors, regulated by Fuzzy Logic. Its input sources include a MPU6050 sensor and data from object detection being run on a Raspberry pi 5. The object detection is executed using YOLOv8, Model trained on Google Colab using datasets obtained from Roboflow.**

its is divide in 2 parts, **The Fuzzy Logic** and the **Object detection** part.
# Part 1: Fuzzy Logic
Fuzzy logic is a mathematical framework that handles imprecise or uncertain information by allowing degrees of truth.


There are majorly in 4 steps in a Fuzzy Logic based System:
1. Mapping inputs to linguistic variables.
2. Applying Fuzzy Sets and Memberships functions to it to quantify the uncertainty.
3. Make decisions based on pre determined rules based on the inputs and
4. Finally, it aggregates the results using fuzzy inference systems to produce precise output.

# Here:

**The Inputs are** : Speed of the Vehicle and The intensity of the instability on road , may it be pothole or a hump.

The speed may vary from 0-120 km/hr value & The Bump intensity would be measured by the MPU6050, a mems IMU.It had a range of -1 to 1.

**The output is** : Stiffness of suspension system. ranging from 0.0 to 1.0



