# Design Overview
## 1. System Details
This project consists of a small two degrees-of-freedom robotic arm capable of performing pick and place tasks. The arm
involves two rotational joints, the shoulder and elbow, and an end effector, the gripper. The system is controlled using an
Arduino Uno and a PCA9685 PWM motor driver which generates the servo control signals. The arm is capable of moving small objects, such as a dice, to from from predetermined positions using scripted joint movements.

## 2. Mechanical Design
The arm consists of two links which rotate about a stationary base. The links are driven at two joints through servo motors.
The gripper attached to the end of the second link is also driven by a servo motor.

The servo motors used are:
- Shoulder Joint: DS3225
- Elbow Joint: MG996R
- Gripper: MG90S

The shoulder joint is driven directly by the servo, while the elbow joint is driven through a pulley system.

## 3. Electrical Design
The system's electronics consist of:
- Arduino Uno microcontroller
- PCA9685 motor driver
- External 6V 10A power supply
- USB power supply

The Arduino Uno and PCA9685 are powered by the USB supply and control the systems logic. The PCA9685 provides PWM signals to the servos which are powered through the external 6V 10A power supply.

## 4. Control Design
The arm is controlled through commands entered into the serial interface which the code interprets and provides PWM commands through the arduino to the PCA9685 to the servos to perform the commanded tasks. The system allows for specific poses and positions to be saved and used to perform different routines, such as the pick and place routine.

## 5. Key Design Decisions
The following design decisions were implemeted to ensure the reliability and performance of the system:
- A PCA9685 motor driver was used to provide accurate and stable servo timing.
- Servo limits were calibrated to prevent mechanical binding and damage to servos.
- A soft-close approach was used to control the gripper and prevent overloading the servo.
- The logic power and servo actuation power were separated into their own circuits to prevent voltage drop and overloading of
  the arduino.
