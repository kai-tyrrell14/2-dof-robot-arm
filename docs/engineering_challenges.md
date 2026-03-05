# Engineering Challenges
This document summarises the main engineering challenges faced during the development of the robotic arm and the solutions that
were implemented.

## 1. Shoulder Torque Limitation
### 1.1 Issue
During the early testing of the shoulder joint, once link 2 was connected the joint struggled when rotated by a low torque servo.
### 1.2 Cause
Adding link 2 increases the torque requirements to rotate the shoulder joint.
### 1.3 Solution
The inital MG996R servo was upgraded to a higher torque DS3225 servo which provided the required torque and ensured stable
motion.

## 2. Gripper Servo Overloaded
### 2.1 Issue
During the initial testing of the gripper servo there was audible strain and eventually the servo failed and needed to be replaced.
### 2.2 Cause
The gripper servo was commanded to close past its limits and when gripping an object it attempts to close to this point which overloads the servo and causes permanent damage.
### 2.3 Solution
A soft-close approach was implemented in the code for gripper which prevents the gripper from loading past its limits and 
releases the servo to prevent overloading.

## 3. Arm Stability
### 3.1 Issue
The arm occasionally tipped forward when elbow was fully extended.
### 3.2 Cause
When fully extended the moment arm is increased which reduces the stability of the arm's base.
### 3.3 Solution
Motion sequencing and smoothing were implemented to increase stability.

## 4. Command Interface
### 4.1 Issue
Early versions of the code required single codes to be entered one at a time.
### 4.2 Cause
The command interface didn't support the entry of multiple commands.
### 4.3 Solution
A command splitter was implemented, allowing for the entry of a series of commands which could be performed sequentially.
