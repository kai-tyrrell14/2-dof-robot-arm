# Stability Notes
This file documents issues regarding stability that were observed throughout the design and testing processes.
## Observed Behaviour
- When the elbow is extended straight ahead the risk of tipping is increased.
- When the gripper is loaded with a heaviour object the risk of tipping is increased.
- Rapid shoulder movements increase the risk of tipping.
## Mitigations 
Serveral strategies were implemented in order to improve stability:
- Motion smoothing
- Motion sequencing
- Increasing base weight

