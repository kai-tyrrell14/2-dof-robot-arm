# 2 DOF Robotic Arm
An Arduino controlled 2-dof robotic arm capable of performing autonomous pick and place commands using calibrated servos and a gripper.
## 1. Demo
![Robot Arm Demo](media/motion_gif.gif)

This demonstrates a simple pick and place routine using scripted joint movements.
## 2. Features
- 2-DOF robotic arm with gripper end effector
- Smooth joint motion control
- Soft-close gripper implemented
- Multi-command serial interface
- External power supply for reliable operation
- Automated pick and place routine

## 3. Hardware
| Component | Description |
|-----------|-------------|
| Arduino Uno | Microcontroller |
| PCA9685 Motor Driver | Servo controller |
| DS3225 Servo | Shoulder joint actuator |
| MG996R Servo | Elbow joint actuator |
| MG90S Servo | Gripper actuator |
| 6V 10A Power Supply | External servo power supply |

Servos are powered through external supply while Arduino and PCA9685 are powered through USB.

## 4. System Design
The system consists of three subsystems:
### 4.1 Mechanical System
A planar system consisting of two links with a shoulder joint and an elbow joint. The gripper is the end effector used to 
pick up and place objects.
### 4.2 Electrical System
An Arduino Uno sends commands to the PCA9685 which sends PWM signals to the servos
### 4.3 Control System
Commands are entered through the serial interface which interprets the commands and converts them into servo actuation.

System diagrams are available in the 'diagrams' folder.

## 5. User Interface
Commands are sent through the Arduino Serial Monitor using the following keys:
| Command | Description |
|---------|-------------|
| 's <μs>' | Move shoulder servo to pulse width (μs) |
| 'e <μs>' | Move elbow servo to pulse width (μs) |
| 'o' | Open gripper |
| 'c' | Close gripper |
| 'h' | Move to home position |
| 'g' | Perform pick and place routine |
| 'p' | Print current joint state |
| 'r' | Release servo PWM |
| 'wait <μs>' | Pause sequence for (μs) |

## 6. Repository Structure
An overview of the files included in this repository:
- **'/CAD':** CAD models of mechanical
- **'/analysis':** Calibration and engineering analysis
- **'/code':** Arduino control software
- **'/diagrams':** System design diagrams
- **'/docs':** Project documentation
- **'/electronics':** Wiring diagrams and pinout reference
- **'/media':** Demomstration images and videos

## 7. Engineering Challenges
A number of issues were encountered and solutions implemented:
- Shoulder joint servo was upgraded from MG996R to DS3225 after torque requirements were not being met
- A soft-close approach was implemented to prevent servo overloading after a servo was damaged.
- Motion smoothing and sequencing was implemented after stability issues were observed.

More information can be found in 'docs/engineering_challenges.md'

## 8. Future Improvements
Some potential improvements may include:
- Improving Base Stability
- Computer Vision Integration
- Automated Path Planning

More information can be found in 'docs/future_improvements.md'
