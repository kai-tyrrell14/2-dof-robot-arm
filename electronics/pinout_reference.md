# Pinout Reference - 2-DOF Robotic Arm
**Controller:** Arduino Uno

**Servo Driver:** PCA9685

## 1. I2C Communication Interface
| Arduino Pin | PCA9685 | Function |
|-------------|---------|----------|
| A4 | SDA | I2C Data |
| A5 | SCL | I2C Clock |
| 5V | VCC | Logic Power |
| GND | GND | Common Ground |

## 2. Servo Channel Assignments
| PCA9685 Channel | Component | Purpose |
|-----------------|-----------|---------|
| CH0 | Shoulder Servo | Base joint rotation |
| CH1 | Elbow Servo | Elbow joint rotation |
| CH2 | Gripper Servo | Gripper operation |

## 3. Power Distribution
| Source | Destination | Description |
|--------|-------------|-------------|
| 6V External Supply | PCA9685 | Servo Power Rail |
| External GND | Arduino GND | Common Ground |
| Arduino 5V | PCA9685 | Driver Logic Power |

## 4. Servo Wiring
| Wire Colour | Signal | Connected To |
|-------------|--------|--------------|
| Red | V+ | PCA9685 V+ Rail |
| Black | GND | PCA9685 GND |
| Yellow | PWM | PCA9685 Channel Output |

