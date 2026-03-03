#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

// Allows the test channel to be easily changed without changing logic.
#define TEST_CH 0

// Boundaries to prevent extreme loading of the servos.
const int HARD_MIN_US = 400;
const int HARD_MAX_US = 2500;

// Using these steps reduces sudden torque spikes.
int stepUs = 10;
int stepDelayMs = 8;


int currentUs = 1000;

// Converts microseconds to ticks to allow for consistent pulse width.
int usToTicks(int us) {
  us = constrain(us, HARD_MIN_US, HARD_MAX_US);
  return (int)((us * 4096L) / 20000L); // 50Hz frame
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
void moveTo(int target) {
  target = constrain(target, HARD_MIN_US, HARD_MAX_US);

  int dir = (target >= currentUs) ? 1 : -1;

  for (int u = currentUs; u != target; u += dir * stepUs) {
    setServoUs(TEST_CH, u);
    delay(stepDelayMs);

    if ((dir > 0 && u + stepUs > target) ||
        (dir < 0 && u - stepUs < target)) break;
  }

  setServoUs(TEST_CH, target);
  currentUs = target;
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

// Command Instructions
void printHelp() {
  Serial.println("\n=== Servo Range Calibration ===");
  Serial.println("+         : increase pulse");
  Serial.println("-         : decrease pulse");
  Serial.println("g <us>    : go to pulse");
  Serial.println("step <us> : set increment");
  Serial.println("delay <ms>: set delay between steps");
  Serial.println("auto      : slow sweep test");
  Serial.println("p         : print current value");
  Serial.println("r         : release PWM");
  Serial.println("================================\n");
}

// The code below takes the input from the user and splits into commands to send to the servo.
void handleLine(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd == "+") {
    moveTo(currentUs + stepUs);
    return;
  }

  if (cmd == "-") {
    moveTo(currentUs - stepUs);
    return;
  }

  if (cmd.equalsIgnoreCase("p")) {
    Serial.print("Current µs: ");
    Serial.println(currentUs);
    return;
  }

  if (cmd.equalsIgnoreCase("r")) {
    releaseServo(TEST_CH);
    Serial.println("PWM released.");
    return;
  }

  if (cmd.equalsIgnoreCase("auto")) {
    Serial.println("Slow sweep starting...");
    for (int u = 600; u <= 2200; u += 20) {
      setServoUs(TEST_CH, u);
      delay(15);
    }
    for (int u = 2200; u >= 600; u -= 20) {
      setServoUs(TEST_CH, u);
      delay(15);
    }
    currentUs = 1000;
    Serial.println("Sweep complete.");
    return;
  }

  int sp = cmd.indexOf(' ');
  String head = (sp >= 0) ? cmd.substring(0, sp) : cmd;
  String rest = (sp >= 0) ? cmd.substring(sp + 1) : "";

  head.trim(); rest.trim();

  if (head.equalsIgnoreCase("g")) {
    int val;
    if (!parseIntArg(rest, val)) {
      Serial.println("Use: g <us>");
      return;
    }
    moveTo(val);
    return;
  }

  if (head.equalsIgnoreCase("step")) {
    int val;
    if (!parseIntArg(rest, val)) {
      Serial.println("Use: step <us>");
      return;
    }
    stepUs = constrain(val, 1, 50);
    Serial.print("stepUs = ");
    Serial.println(stepUs);
    return;
  }

  if (head.equalsIgnoreCase("delay")) {
    int val;
    if (!parseIntArg(rest, val)) {
      Serial.println("Use: delay <ms>");
      return;
    }
    stepDelayMs = constrain(val, 0, 50);
    Serial.print("delayMs = ");
    Serial.println(stepDelayMs);
    return;
  }

  Serial.println("Unknown command. Type help.");
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  Serial.println("Servo Range Test Ready.");
  printHelp();

  moveTo(currentUs);
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      String cmd = line;
      line = "";
      handleLine(cmd);
    } else if (c != '\r') {
      line += c;
      if (line.length() > 80)
        line = line.substring(0, 80);
    }
  }
}