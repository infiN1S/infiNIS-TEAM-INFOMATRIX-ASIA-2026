#include <Bluepad32.h>

#define DEADZONE 10

// ===== MOTOR PINS =====
#define IN1 27
#define IN2 26
#define IN3 14
#define IN4 12
#define IN5 19
#define IN6 21
#define IN7 2
#define IN8 4

ControllerPtr ctl = nullptr;

// ===== MAX SPEED SETTINGS =====
const int speedLevels[4] = {100, 150, 200, 255};
int speedIndex = 2; // стартовая скорость = 150
int MAX_SPEED = speedLevels[speedIndex];

// ===== BUTTON EDGE DETECTION =====
bool prevL1 = false;
bool prevR1 = false;

void onConnectedController(ControllerPtr c) { ctl = c; }
void onDisconnectedController(ControllerPtr c) { if (ctl == c) ctl = nullptr; }

void setMotor(int pinA, int pinB, int speed) {
  speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
  if (speed >= 0) {
    analogWrite(pinA, speed);
    analogWrite(pinB, 0);
  } else {
    analogWrite(pinA, 0);
    analogWrite(pinB, -speed);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
}

void loop() {
  BP32.update();

  if (ctl && ctl->isConnected()) {
    bool currL1 = ctl->l1();
    bool currR1 = ctl->r1();

    bool l1Pressed = currL1 && !prevL1;
    bool r1Pressed = currR1 && !prevR1;

    prevL1 = currL1;
    prevR1 = currR1;

    // ===== SPEED CONTROL =====
    if (l1Pressed && !currR1) {
      speedIndex = max(0, speedIndex - 1);
      MAX_SPEED = speedLevels[speedIndex];
    }
    if (r1Pressed && !currL1) {
      speedIndex = min(3, speedIndex + 1);
      MAX_SPEED = speedLevels[speedIndex];
    }
    if (currL1 && currR1) {
      speedIndex = 3;
      MAX_SPEED = 255;
    }

    int lx = ctl->axisY();
    int ly = ctl->axisX();
    int rx = ctl->axisRY();
    int ry = ctl->axisRX();

    if (abs(lx) < DEADZONE) lx = 0;
    if (abs(ly) < DEADZONE) ly = 0;
    if (abs(rx) < DEADZONE) rx = 0;
    if (abs(ry) < DEADZONE) ry = 0;

    lx = map(lx, -512, 512, -MAX_SPEED, MAX_SPEED);
    ly = map(ly, -512, 512, -MAX_SPEED, MAX_SPEED);
    rx = map(rx, -512, 512, -MAX_SPEED, MAX_SPEED);
    ry = map(ry, -512, 512, -MAX_SPEED, MAX_SPEED);

    int FR = constrain(ly + lx + ry - rx, -MAX_SPEED, MAX_SPEED);
    int FL = constrain(ly - lx + ry + rx, -MAX_SPEED, MAX_SPEED);
    int BR = constrain(ly - lx + ry - rx, -MAX_SPEED, MAX_SPEED);
    int BL = constrain(ly + lx + ry + rx, -MAX_SPEED, MAX_SPEED);

    setMotor(IN1, IN2, FR);
    setMotor(IN5, IN6, FL);
    setMotor(IN3, IN4, BR);
    setMotor(IN7, IN8, BL);
  }
}