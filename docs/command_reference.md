# Command Reference
This document outlines how to control the arm through the serial interface.
## 1. Command Format
Commands to the arm are entered through Arduino's Serial Monitor. Multiple commands can be performed in sequence by separating
them  with ':'.

For example:

s 1400; e 1200; h

## 2. Supported Commands
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

## 3. Example Commands
s 1400; e 1300; o;

Moves shoulder and elbow to respective positions and opens gripper.


s 1700; e 1600; h; g
Moves shoulder and elbow to respective positions, returns home, performs pick and place routine.
