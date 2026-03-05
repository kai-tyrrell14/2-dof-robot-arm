# 2-dof-robot-arm
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
