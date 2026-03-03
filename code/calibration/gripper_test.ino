#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

// Channel mapped as per 'pinout_reference.md' so to change channel only this value 
// must be changed.
#define GRIPPER_CH 2

// Limits to determine the upper and lower bounds
const int SERVO_US_MIN = 400;
const int SERVO_US_MAX = 2500;

// Using these steps reduces sudden torque spikes.
int stepUs = 5;
int stepDelayMs = 6;

int gr_us = 700;

// Converts microseconds to ticks to allow for consistent pulse width.
int usToTicks(int us) {
  us = constrain(us, SERVO_US_MIN, SERVO_US_MAX);
  return (int)((us * 4096L) / 20000L); // 50Hz => 20,000 us
}

// Uses the value above to set the PWM for the servo, where the limits and positions
// above can be changed without changing logic below.
void setServoUs(int ch, int us) {
  pwm.setPWM(ch, 0, usToTicks(us));
}

// Releases servo to prevent overloading.
void releaseServo(int ch) {
  pwm.setPWM(ch, 0, 0);
}

// Using the stepped motion below, sudden acceleration and peak torque are reduced.
void moveToUs(int ch, int &current, int target) {
  target = constrain(target, SERVO_US_MIN, SERVO_US_MAX);

  int dir = (target >= current) ? 1 : -1;
  for (int u = current; u != target; u += dir * stepUs) {
    setServoUs(ch, u);
    delay(stepDelayMs);

    // prevent overshoot with step size
    if ((dir > 0 && u + stepUs > target) || (dir < 0 && u - stepUs < target)) break;
  }
  setServoUs(ch, target);
  current = target;
}

// Command Instructions
void printHelp() {
  Serial.println("\n=== Gripper Test (PCA9685 CH2) ===");
  Serial.println("Commands:");
  Serial.println("  o <us>      : go to OPEN value (example: o 500)");
  Serial.println("  c <us>      : go to CLOSE value (example: c 950)");
  Serial.println("  g <us>      : go to any pulse (example: g 750)");
  Serial.println("  step <us>   : set step size (example: step 5)");
  Serial.println("  delay <ms>  : set step delay (example: delay 6)");
  Serial.println("  r           : release PWM (stop holding torque)");
  Serial.println("  p           : print current value");
  Serial.println("  help        : show this help");
  Serial.println("Tip: Set Serial Monitor line ending = Newline");
  Serial.println("=================================\n");
}

String line;

bool parseIntArg(String s, int &out) {
  s.trim();
  if (s.length() == 0) return false;
  long v = s.toInt();
  if (v == 0 && s != "0") return false;
  out = (int)v;
  return true;
}

// The code below takes the input from the user and splits into commands to send to the servo.
void handleLine(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  int sp = cmd.indexOf(' ');
  String head = (sp >= 0) ? cmd.substring(0, sp) : cmd;
  String rest = (sp >= 0) ? cmd.substring(sp + 1) : "";
  head.trim(); rest.trim();

  if (head.equalsIgnoreCase("help") || head.equals("?")) { printHelp(); return; }
  if (head.equalsIgnoreCase("p")) {
    Serial.print("gr_us="); Serial.print(gr_us);
    Serial.print("  stepUs="); Serial.print(stepUs);
    Serial.print("  stepDelayMs="); Serial.println(stepDelayMs);
    return;
  }
  if (head.equalsIgnoreCase("r")) { releaseServo(GRIPPER_CH); Serial.println("Released gripper PWM"); return; }

  if (head.equalsIgnoreCase("step")) {
    int v; if (!parseIntArg(rest, v)) { Serial.println("step needs number"); return; }
    stepUs = constrain(v, 1, 50);
    Serial.print("stepUs="); Serial.println(stepUs);
    return;
  }
  if (head.equalsIgnoreCase("delay")) {
    int v; if (!parseIntArg(rest, v)) { Serial.println("delay needs number"); return; }
    stepDelayMs = constrain(v, 0, 50);
    Serial.print("stepDelayMs="); Serial.println(stepDelayMs);
    return;
  }

  if (head.equalsIgnoreCase("o") || head.equalsIgnoreCase("c") || head.equalsIgnoreCase("g")) {
    int target;
    if (!parseIntArg(rest, target)) { Serial.println("Command needs <us>"); return; }
    target = constrain(target, SERVO_US_MIN, SERVO_US_MAX);

    Serial.print("Moving gripper -> "); Serial.println(target);
    moveToUs(GRIPPER_CH, gr_us, target);

    Serial.print("Now at "); Serial.println(gr_us);
    return;
  }

  Serial.print("Unknown command: "); Serial.println(cmd);
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  Serial.println("Gripper test ready.");
  printHelp();

  moveToUs(GRIPPER_CH, gr_us, gr_us);
  releaseServo(GRIPPER_CH);
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      String cmd = line; line = "";
      handleLine(cmd);
    } else if (c != '\r') {
      line += c;
      if (line.length() > 80) line = line.substring(0, 80);
    }
  }
}