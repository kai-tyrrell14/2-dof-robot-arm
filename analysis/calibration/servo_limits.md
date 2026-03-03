# Servo Limits Calibration
All limits were determined experimentally using incremental increases to determine where overloading occurs.

## Summary Table
| Joint | Channel | Min (μs) | Max (μs) | Home (μs) | 
|-------|---------|----------|----------|-----------|
| Shoulder | CH0 | 700 | 2300 | 1200 |
| Elbow | CH1 | 700 | 1700 | 800 |
| Gripper | CH2 | 500 (open) | 950 (soft-close) | 950 |

## Calibration Method
1. Servo was moved in small increments (5-10 μs).
2. Movement was stopped as soon as binding occured.
3. Limits were set 50 μs from these points to ensure safe limits.
4. These limits were then validated through repeated motion.
5. These final values were set as constraints in the code.

## Design Intent

These servo limits are clearly defined and documented in order to:
- Prevent mechanical binding
- Prevent servo overloading
- Prevent gear stripping
- Ensure system reliability
