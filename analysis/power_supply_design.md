# Power Supply Design
This document outlines the power requirements for the entire robotic arm system. The system incorporates three different servo
types for the shoulder, elbow and gripper.

## 1.System Components
| Component | Supply | Notes |
|-----------|--------|-------|
| Arduino Uno | USB | Microcontroller |
| PCA9685 | Arduino 5V | PWM Driver |
| Shoulder Servo | 6V External | DS3225 Servo |
| Elbow Servo | 6V External | MG996R Servo |
| Gripper Servo | 6V External | MG90S Servo |

## 2. Estimated Servo Current
Current drawn by servos varies depending on load.
| Condition | DsS3225 (Shoulder) | MG996R (Elbow) | MG90S (Gripper) |
|-----------|-------------------|----------------|-----------------|
| Idle | ~20-50 mA | ~10-50 mA | ~10-20 mA |
| Moving (no load) | ~500 mA | ~500-900 mA | ~150-250 mA |
| Under Load | ~1.5 A | ~1.5-2 A | ~400-600 mA |
| Stall (worst case) | ~2.3 A | ~2.5 A | ~700 mA |

## 3. Worst Case Design Scenario
The worst case design scenario assumes all servos acting at stall conditions.
Worst case per servo:
- Shoulder (DS3225) = 2.3 A
- Elbow (MG996R) = 2.5 A
- Gripper (MG90S) = 0.7 A

Total peak demand:
2.3 A + 2.5 A + 0.7 A = 5.5 A

## 4. Logic Power Requirements
| Component | Current |
|-----------|---------|
| Arduino Uno | ~50 mA |
| PCA9685 | ~10 mA |
Total pwer requirement ≈ 60 mA

## 5. Final Power Supply Design 
The final design for delivering power to the robotic arm involves separating servo actuation power and logic power into two
different circuits. This prevents voltage drop and ensures the Arduino Uno or PCA9685 do not draw excessive current. The power
must also safely meet the demands of the worst case design scenario.

The following power supplys are used in the system:
- Servo Actuation Power: External 6V 10A power supply
- Logic Power: USB powered
