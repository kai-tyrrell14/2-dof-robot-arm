# Calibration Results
This document summarises the methods for calibrating the servos used for the robotic arm. 
This calibration was necessary before automated routines, such as pick and place, can be performed.

## 1. Test Method
- Controller: Arduino Uno + PCA9685
- Method: incremental pulse stepping
- Observation criteria: mechanical binding, audible strain, overheating
- Validation: repeated motion cycles without any issues

Final operating limits shown in 'servo_limits.md'
## Calibration of Servos
### 2.1 Shoulder (CH0)
**Experimental range:** 600-2400 μs

**Key observations:**
- Binding observed below 640 μs
- Binding observed above 2350 μs
- Shoulder movement at high speeds with elbow fully extended may result in tipping

**Validation:**
- Repeated motion cycles within servo limits resulted in no binding

### 2.2 Elbow (CH1)
**Experimental range:** 600-1800 μs

**Key observations:**
- Increasing the pulse width moves elbow downwards
- Binding observed below 650 μs
- Binding observed above 1770 μs

**Validation:**
- Repeated motion cycles within servo limits resulted in no binding

### 2.3 Gripper (CH2)
**Initial issue:**
- Original command for closing gripper resulted in sustained overloading of the SG90 servo
- As a result one servo failed from being overloaded for too long
- Gripper was driven to stall torque near limit

**Resolution:**
- A soft-close was implemented which closes the gripper enough to grip an object, before releasing the servo to prevent extended overloading

**Validation:**
- Repeated grip and release cycles with soft-close without failure
