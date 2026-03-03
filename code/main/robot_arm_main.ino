#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

// Channels
// Channels mapped as per 'pinout_reference.md' so to change channels only these values 
// must be changed.
#define SHOULDER_CH 0
#define ELBOW_CH    1
#define GRIPPER_CH  2

// Limits
// Limits have been determined experimentally, where going past these limits can overload
// servos.
const int SHOULDER_MIN_US = 700;
const int SHOULDER_MAX_US = 2300;

const int ELBOW_MIN_US = 700;
const int ELBOW_MAX_US = 1700;

const int GRIP_OPEN_US       = 500;
const int GRIP_CLOSE_SOFT_US = 950;

// Home Position
// This position is used as a reference for testing and demonstrations.
const int SHOULDER_HOME_US = 1200;
const int ELBOW_HOME_US    = 800;

// The auto settings allow for faster movements to known positions, such as home or pick
// and place. The manual settings are slower for manual inputs.
const int AUTO_STEP_US   = 10;  
const int AUTO_DELAY_MS  = 10;

const int MANUAL_STEP_US  = 5;  
const int MANUAL_DELAY_MS = 4;

// Initial Position
int sh_us = SHOULDER_HOME_US;
int el_us = ELBOW_HOME_US;
int gr_us = GRIP_CLOSE_SOFT_US;

// Poses
struct Pose { int shoulder; int elbow; };

Pose HOME       = { 1200,  800 };
Pose ABOVE_PICK = { 1200, 1200 };  
Pose PICK_DOWN  = { 1200, 1700 };  

Pose ABOVE_PLACE= { 1500, 1200 };  
Pose PLACE_DOWN = { 1500, 1700 };

Pose LIFT_PICK  = { 1200, 1050 };  
Pose LIFT_PLACE = { 1500, 1050 };  


// Converts microseconds to ticks to allow for consistent pulse widths through each channel.
int usToTicks(int us) {
  us = constrain(us, 400, 2500);
  return (int)((us * 4096L) / 20000L);  
}

// Uses the value above to set the PWM for the servos, where the limits and positions
// above can be changed with changing logic below.
void setServoUs(int ch, int us) {
  pwm.setPWM(ch, 0, usToTicks(us));
}

// Releases servo to prevent overloading.
void releaseServo(int ch) {
  pwm.setPWM(ch, 0, 0);
}

// Using the stepped motion below, sudden acceleration and peak torque are reduced.
void moveTo(int ch, int &current, int target, int stepUs, int delayMs) {
  target = constrain(target, 400, 2500);

  if (target > current) {
    for (int u = current; u <= target; u += stepUs) {
      setServoUs(ch, u);
      delay(delayMs);
    }
  } else {
    for (int u = current; u >= target; u -= stepUs) {
      setServoUs(ch, u);
      delay(delayMs);
    }
  }
  current = target;
}

void printState() {
  Serial.print("Shoulder_us="); Serial.print(sh_us);
  Serial.print("  Elbow_us=");  Serial.print(el_us);
  Serial.print("  Gripper_us=");Serial.println(gr_us);
}

// The code below sets the order of movement. The elbow is moved before the shoulder to 
// reduce the risk of the arm tipping.
void moveArm(Pose p, bool elbowFirst=true) {
  p.shoulder = constrain(p.shoulder, SHOULDER_MIN_US, SHOULDER_MAX_US);
  p.elbow    = constrain(p.elbow,    ELBOW_MIN_US,    ELBOW_MAX_US);

  if (elbowFirst) {
    moveTo(ELBOW_CH,    el_us, p.elbow,    AUTO_STEP_US, AUTO_DELAY_MS);
    moveTo(SHOULDER_CH, sh_us, p.shoulder, AUTO_STEP_US, AUTO_DELAY_MS);
  } else {
    moveTo(SHOULDER_CH, sh_us, p.shoulder, AUTO_STEP_US, AUTO_DELAY_MS);
    moveTo(ELBOW_CH,    el_us, p.elbow,    AUTO_STEP_US, AUTO_DELAY_MS);
  }
}

void gripperOpen() {
  moveTo(GRIPPER_CH, gr_us, GRIP_OPEN_US, AUTO_STEP_US, AUTO_DELAY_MS);
  releaseServo(GRIPPER_CH);
}

// This function ensures that the gripper is able to pick up an object with overloading and
// damaging the servo.
void gripperCloseSoft(int holdMs = 150) {
  int approach = (GRIP_OPEN_US + GRIP_CLOSE_SOFT_US) / 2;
  moveTo(GRIPPER_CH, gr_us, approach, AUTO_STEP_US, AUTO_DELAY_MS);
  delay(60);

  moveTo(GRIPPER_CH, gr_us, GRIP_CLOSE_SOFT_US, AUTO_STEP_US, AUTO_DELAY_MS);
  // This delay ensures that the gripper properly grips the object.
  delay(holdMs);
  // This release prevents holding for too long and overloading.
  releaseServo(GRIPPER_CH);
}

// Pick and Place Routine
// Repeatable sequence for testing and demonstration.
void runPickPlace() {
  moveArm(ABOVE_PICK, true);  delay(150);
  gripperOpen();              delay(120);

  moveArm(PICK_DOWN, false);  delay(180);
  gripperCloseSoft(180);      delay(120);

  moveArm(LIFT_PICK, true);   delay(150);

  moveArm(ABOVE_PLACE, true); delay(150);

  moveArm(PLACE_DOWN, false); delay(180);
  gripperOpen();              delay(150);

  moveArm(LIFT_PLACE, true);  delay(150);

  moveArm(HOME, true);        delay(150);
  gripperCloseSoft(150);

  releaseServo(SHOULDER_CH);
  releaseServo(ELBOW_CH);
}

// Manual Servo Operation
void gotoShoulderUs(int targetUs) {
  targetUs = constrain(targetUs, SHOULDER_MIN_US, SHOULDER_MAX_US);
  moveTo(SHOULDER_CH, sh_us, targetUs, MANUAL_STEP_US, MANUAL_DELAY_MS);
}

void gotoElbowUs(int targetUs) {
  targetUs = constrain(targetUs, ELBOW_MIN_US, ELBOW_MAX_US);
  moveTo(ELBOW_CH, el_us, targetUs, MANUAL_STEP_US, MANUAL_DELAY_MS);
}


// The code below allows for the commanding of positions to move the robotic arm.
String line;

String trimCopy(String s) { s.trim(); return s; }

bool parseNumberAfter(const String &s, int &out) {
  String t = s;
  t.trim();
  if (t.length() == 0) return false;
  long v = t.toInt();
  if (v == 0 && t != "0") return false;
  out = (int)v;
  return true;
}

void execOne(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  cmd.replace("=", " ");
  cmd.trim();

  int sp = cmd.indexOf(' ');
  String head = (sp >= 0) ? cmd.substring(0, sp) : cmd;
  String rest = (sp >= 0) ? cmd.substring(sp + 1) : "";
  head.trim(); rest.trim();

  if (head.equalsIgnoreCase("help") || head.equals("?")) {
    Serial.println("Use: s <us>; e <us>; o; c; g; h; p; r; wait <ms>");
    return;
  }
  if (head.equalsIgnoreCase("p")) { printState(); return; }
  if (head.equalsIgnoreCase("o")) { gripperOpen(); return; }
  if (head.equalsIgnoreCase("c")) { gripperCloseSoft(150); return; }
  if (head.equalsIgnoreCase("g")) { runPickPlace(); return; }
  if (head.equalsIgnoreCase("h")) {
    moveArm(HOME, true);
    gripperCloseSoft(150);
    releaseServo(SHOULDER_CH);
    releaseServo(ELBOW_CH);
    return;
  }
  if (head.equalsIgnoreCase("r")) {
    releaseServo(SHOULDER_CH);
    releaseServo(ELBOW_CH);
    return;
  }

  if (head.equalsIgnoreCase("wait") || head.equalsIgnoreCase("w")) {
    int ms;
    if (!parseNumberAfter(rest, ms)) { Serial.println("wait needs ms, e.g. wait 500"); return; }
    ms = constrain(ms, 0, 20000);
    delay(ms);
    return;
  }

  if (head.equalsIgnoreCase("s")) {
    int target;
    if (!parseNumberAfter(rest, target)) { Serial.println("s needs a number, e.g. s 1200"); return; }
    gotoShoulderUs(target);
    return;
  }
  if (head.equalsIgnoreCase("e")) {
    int target;
    if (!parseNumberAfter(rest, target)) { Serial.println("e needs a number, e.g. e 1200"); return; }
    gotoElbowUs(target);
    return;
  }

  Serial.print("Unknown cmd: "); Serial.println(cmd);
}

// This code allows for multiple commands to be entered at once, which will be commanded 
// in order.
void execSequence(String seq) {
  seq.replace(",", ";");

  int start = 0;
  while (start < (int)seq.length()) {
    int end = seq.indexOf(';', start);
    if (end < 0) end = seq.length();

    String cmd = seq.substring(start, end);
    cmd.trim();
    if (cmd.length() > 0) {
      execOne(cmd);
    }
    start = end + 1;
  }
}

// Command Instructions
void printHelp() {
  Serial.println("=== Multi-command control ===");
  Serial.println("Separate commands with ';' (or ','). Examples:");
  Serial.println("  s 1400; e 1200; o");
  Serial.println("  s=1200; e=1700; c; wait 500; s 1500; e 1200; o");
  Serial.println("Commands: s <us>, e <us>, o, c, g, h, p, r, wait <ms>");
  Serial.println("Note: set Serial Monitor line ending = Newline (or Both NL&CR).");
  Serial.println("=============================");
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  Serial.println("Robot Arm Ready (multi-command + pick/place).");
  printHelp();

  // This starts the arm in the home position for ease of testing and demonstration.
  moveArm(HOME, true);
  delay(150);
  gripperCloseSoft(150);
  printState();
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      String cmdLine = line;
      line = "";
      cmdLine.trim();
      if (cmdLine.length() > 0) {
        execSequence(cmdLine);
      }
    } else if (c != '\r') {
      line += c;
      if (line.length() > 140) line = line.substring(0, 140);
    }
  }
}
