# Testing Validation
This document outlines he methods to verify the system works as expected.
## 1. Testing Objectives
The objectives that were tested and verified:
- Each joint could move through its calibrated servo limits
- Joint movements were performed smoothly
- Small objects could be gripped and released
- The pick and place routine was performed correctly
- The system can operate without any damage to or overheating of servos

## 2. Testing Procedures
The system was tested through the serial interface.

Manual commands were entered to test each servo through their calibrated ranges and multiple open and close gripper commands
were performed.

Repeated automated pick and place routines were then executed to ensure motion and positioning were accurate and repeatable.

## 3. Testing Results
| Test | Descritption | Results |
|------|--------------|---------|
| Joint Movement | Shoulder and elbow moved through calibrated servo limits| Pass |
| Smooth Motion | Servos moved smoothly without sudden jumps | Pass |
| Gripper Operation | Object successfully gripped and released | Pass |
| Pick and Place Routine | The pick and place routine performed multiple times in succession | Pass |
