# Power Distribution
**Controller:** Arduino Uno

**Servo Driver:** PCA9685

**Servo Supply:** 6V DC (10A)

## 1. Overview
The system is powered through an external 6V power supply and the arduino logic is powered through USB.
All grounds are connected and provide a common reference for PWM signalling.

See 'wiring_diagram.png' for the wiring layout.

## 2. Power Sources
| Subsystem | Voltage | Source | Notes |
|-----------|---------|--------|-------|
| Arduino Uno | 5V | USB | Logic |
| PCA9685 Logic | 5V | Arduino 5V -> VCC | I2C + PWM |
| Servos | 6V | External supply -> V+ | High current load |

## 3. Power Wiring
### 3.1 Servo Power
- External +6V -> PCA9685 V+
- PCA9685 V+ rail -> Servo V+

## 3.2 Ground / Reference
- External supply GND -> PCA9685 GND
- Arduino GND -> PCA9685 GND
- Servo GND -> PCA9685 GND

## 4. Risk Controls
### 4.1 Brownout Prevention
- Servo power separate from logic power
- External supply provides stable current during peak loads

### 4.2 Gripper Overheating Prevention
- Soft-close grip implemented
- PWM is released after gripper to avoid continous stall current
