#include <Arduino.h>
#include <WiFi.h>
#include <SettingsGyver.h>
#include <GyverDBFile.h>
#include <LittleFS.h>

// ==== WiFi (ESP32 creates AP) ====
#define WIFI_SSID "ESP32_CAR"
#define WIFI_PASS "12345678"  // 8+ chars for WPA2

struct Motor {
  uint8_t pinA;
  uint8_t pinB;
  uint8_t chanA;
  uint8_t chanB;
  bool invert;
};

Motor mFrontLeft{19, 21, 0, 1, false};
Motor mFrontRight{26, 27, 2, 3, false};
Motor mRearLeft{33, 32, 4, 5, false};
Motor mRearRight{14, 13, 6, 7, false};


// ==== PWM config ====
static const uint32_t PWM_FREQ = 20000;
static const uint8_t PWM_BITS = 8;
static const uint16_t PWM_MAX = (1 << PWM_BITS) - 1;

// ==== LEDC compatibility (ESP32 core v2 vs v3) ====
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
static inline void ledcSetupChannel(uint8_t pin, uint8_t channel) {
  ledcAttachChannel(pin, PWM_FREQ, PWM_BITS, channel);
}
static inline void ledcWriteChan(uint8_t channel, uint8_t pin, uint32_t duty) {
  (void)pin;
  ledcWriteChannel(channel, duty);
}
#else
static inline void ledcSetupChannel(uint8_t pin, uint8_t channel) {
  ledcSetup(channel, PWM_FREQ, PWM_BITS);
  ledcAttachPin(pin, channel);
}
static inline void ledcWriteChan(uint8_t channel, uint8_t pin, uint32_t duty) {
  (void)pin;
  ledcWrite(channel, duty);
}
#endif

// ==== Motor pins ====

// ==== Control params ====
uint8_t maxSpeed = 100;  // 0..100
uint8_t maxYaw = 100;    // 0..100
uint8_t deadzone = 5;    // percent 0..100
uint8_t expo = 20;       // percent 0..100
uint8_t minPwm = 10;     // percent 0..100
bool motorsEnabled = true;
bool testActive = false;
uint8_t testStep = 0;
uint32_t testTimer = 0;
bool singleTestActive = false;
uint8_t singleTestMotor = 0;
uint8_t singleTestStep = 0;
uint32_t singleTestTimer = 0;
uint8_t mixMode = 0;
bool invertX = true;
bool invertY = true;
bool invertR = true;
uint32_t lastClientMs = 0;
const uint32_t CLIENT_TIMEOUT_MS = 5000;
const uint32_t LED_BLINK_MS = 500;
#if defined(LED_BUILTIN)
const uint8_t STATUS_LED_PIN = LED_BUILTIN;
#else
const uint8_t STATUS_LED_PIN = 2;
#endif
const bool STATUS_LED_ACTIVE_LOW = false;

// ==== Settings UI + persistent storage ====
GyverDBFile db(&LittleFS, "/settings.db");
SettingsGyver sett(WIFI_SSID, &db);
sets::Pos joyMove;
sets::Pos joyRotate;
bool showSettings = false;
bool resetRequested = false;

// Editable motor pins (UI)
uint8_t pinFrontLeftA = 19, pinFrontLeftB = 21;
uint8_t pinFrontRightA = 26, pinFrontRightB = 27;
uint8_t pinRearLeftA = 33, pinRearLeftB = 32;
uint8_t pinRearRightA = 14, pinRearRightB = 13;

DB_KEYS(
  kk,
  kMotorsEnabled,
  kMaxSpeed,
  kMaxYaw,
  kDeadzone,
  kExpo,
  kMinPwm,
  kMixMode,
  kInvertX,
  kInvertY,
  kInvertR,
  kInvFrontLeft,
  kInvFrontRight,
  kInvRearLeft,
  kInvRearRight,
  kPinFrontLeftA,
  kPinFrontLeftB,
  kPinFrontRightA,
  kPinFrontRightB,
  kPinRearLeftA,
  kPinRearLeftB,
  kPinRearRightA,
  kPinRearRightB,
  kTestMotor
);

// ---- Utility ----
static inline float clamp1(float v) {
  if (v > 1.0f) return 1.0f;
  if (v < -1.0f) return -1.0f;
  return v;
}

float applyDeadzone(float v, float dz) {
  float av = fabsf(v);
  if (av <= dz) return 0.0f;
  float s = (v < 0.0f) ? -1.0f : 1.0f;
  float out = (av - dz) / (1.0f - dz);
  return s * out;
}

float applyExpo(float v, float e) {
  // e in [0..1]
  return (1.0f - e) * v + e * v * v * v;
}

static inline void rotate90CW(float& x, float& y) {
  float nx = y;
  float ny = -x;
  x = nx;
  y = ny;
}

bool isValidPin(uint8_t pin) {
  const uint8_t allowed[] = {13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
  for (uint8_t i = 0; i < sizeof(allowed); i++) {
    if (pin == allowed[i]) return true;
  }
  return false;
}

bool pinsUnique(uint8_t* pins, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    for (uint8_t j = i + 1; j < count; j++) {
      if (pins[i] == pins[j]) return false;
    }
  }
  return true;
}

void detachMotors() {
  ledcDetach(mFrontLeft.pinA);
  ledcDetach(mFrontLeft.pinB);
  ledcDetach(mFrontRight.pinA);
  ledcDetach(mFrontRight.pinB);
  ledcDetach(mRearLeft.pinA);
  ledcDetach(mRearLeft.pinB);
  ledcDetach(mRearRight.pinA);
  ledcDetach(mRearRight.pinB);
}

bool applyMotorPins(bool initial = false) {
  uint8_t pins[8] = {pinFrontLeftA, pinFrontLeftB, pinFrontRightA, pinFrontRightB, pinRearLeftA, pinRearLeftB, pinRearRightA, pinRearRightB};
  for (uint8_t i = 0; i < 8; i++) {
    if (!isValidPin(pins[i])) return false;
  }
  if (!pinsUnique(pins, 8)) return false;

  if (!initial) detachMotors();

  mFrontLeft.pinA = pinFrontLeftA; mFrontLeft.pinB = pinFrontLeftB;
  mFrontRight.pinA = pinFrontRightA; mFrontRight.pinB = pinFrontRightB;
  mRearLeft.pinA = pinRearLeftA; mRearLeft.pinB = pinRearLeftB;
  mRearRight.pinA = pinRearRightA; mRearRight.pinB = pinRearRightB;

  pinMode(mFrontLeft.pinA, OUTPUT); pinMode(mFrontLeft.pinB, OUTPUT);
  pinMode(mFrontRight.pinA, OUTPUT); pinMode(mFrontRight.pinB, OUTPUT);
  pinMode(mRearLeft.pinA, OUTPUT); pinMode(mRearLeft.pinB, OUTPUT);
  pinMode(mRearRight.pinA, OUTPUT); pinMode(mRearRight.pinB, OUTPUT);
  digitalWrite(mFrontLeft.pinA, LOW); digitalWrite(mFrontLeft.pinB, LOW);
  digitalWrite(mFrontRight.pinA, LOW); digitalWrite(mFrontRight.pinB, LOW);
  digitalWrite(mRearLeft.pinA, LOW); digitalWrite(mRearLeft.pinB, LOW);
  digitalWrite(mRearRight.pinA, LOW); digitalWrite(mRearRight.pinB, LOW);

  setupMotors();
  return true;
}

void motorWrite(const Motor& m, float value) {
  if (!motorsEnabled) {
    ledcWriteChan(m.chanA, m.pinA, 0);
    ledcWriteChan(m.chanB, m.pinB, 0);
    return;
  }
  float v = m.invert ? -value : value;
  v = clamp1(v);
  float av = fabsf(v);
  uint16_t duty = (uint16_t)(av * PWM_MAX);
  if (av > 0.001f) {
    uint16_t minDuty = (uint16_t)((minPwm / 100.0f) * PWM_MAX);
    if (duty < minDuty) duty = minDuty;
  }
  if (v >= 0.0f) {
    ledcWriteChan(m.chanA, m.pinA, duty);
    ledcWriteChan(m.chanB, m.pinB, 0);
  } else {
    ledcWriteChan(m.chanA, m.pinA, 0);
    ledcWriteChan(m.chanB, m.pinB, duty);
  }
}

void mixAndDrive(float x, float y, float rot) {
  float fl = y + x + rot;
  float fr = y - x - rot;
  float rl = y - x + rot;
  float rr = y + x - rot;

  float maxv = max(max(fabsf(fl), fabsf(fr)), max(fabsf(rl), fabsf(rr)));
  if (maxv > 1.0f) {
    fl /= maxv;
    fr /= maxv;
    rl /= maxv;
    rr /= maxv;
  }

  motorWrite(mFrontLeft, fl);
  motorWrite(mFrontRight, fr);
  motorWrite(mRearLeft, rl);
  motorWrite(mRearRight, rr);
}

void mixAndDriveAlt(float lx, float ly, float rx, float ry) {
  float fr = ly + lx + ry - rx;
  float fl = ly - lx + ry + rx;
  float rr = ly - lx + ry - rx;
  float rl = ly + lx + ry + rx;

  float maxv = max(max(fabsf(fl), fabsf(fr)), max(fabsf(rl), fabsf(rr)));
  if (maxv > 1.0f) {
    fl /= maxv;
    fr /= maxv;
    rl /= maxv;
    rr /= maxv;
  }

  motorWrite(mFrontLeft, fl);
  motorWrite(mFrontRight, fr);
  motorWrite(mRearLeft, rl);
  motorWrite(mRearRight, rr);
}

void runMotorTest() {
  if (!testActive && !singleTestActive) return;

  if (singleTestActive) {
    uint32_t now = millis();
    if (now - singleTestTimer >= 1000) {
      singleTestTimer = now;
      singleTestStep++;
      if (singleTestStep >= 2) {
        singleTestActive = false;
        motorWrite(mFrontLeft, 0);
        motorWrite(mFrontRight, 0);
        motorWrite(mRearLeft, 0);
        motorWrite(mRearRight, 0);
        return;
      }
    }
    Motor* motors[4] = {&mFrontLeft, &mFrontRight, &mRearLeft, &mRearRight};
    float dir = (singleTestStep == 0) ? 1.0f : -1.0f;
    motorWrite(mFrontLeft, 0);
    motorWrite(mFrontRight, 0);
    motorWrite(mRearLeft, 0);
    motorWrite(mRearRight, 0);
    motorWrite(*motors[singleTestMotor], dir);
    return;
  }

  uint32_t now = millis();
  if (now - testTimer >= 1000) {
    testTimer = now;
    testStep++;
    if (testStep >= 8) {
      testActive = false;
      motorWrite(mFrontLeft, 0);
      motorWrite(mFrontRight, 0);
      motorWrite(mRearLeft, 0);
      motorWrite(mRearRight, 0);
      return;
    }
  }

  Motor* motors[4] = {&mFrontLeft, &mFrontRight, &mRearLeft, &mRearRight};
  uint8_t idx = testStep / 2;
  float dir = (testStep % 2 == 0) ? 1.0f : -1.0f;

  motorWrite(mFrontLeft, 0);
  motorWrite(mFrontRight, 0);
  motorWrite(mRearLeft, 0);
  motorWrite(mRearRight, 0);
  motorWrite(*motors[idx], dir);
}

void buildUI(sets::Builder& b) {
  lastClientMs = millis();
  if (!showSettings) {
    b.Paragraph("", "Joysticks");
    { sets::Row r(b); }
    b.Joystick(joyMove, true);
    b.Joystick(joyRotate, true);

    { sets::Row r(b); }
    if (b.Button("Settings")) {
      showSettings = true;
      sett.reload();
    }
  } else {
    b.Paragraph("", "Settings");
    b.Switch(kk::kMotorsEnabled, "Motors", &motorsEnabled);
    b.Slider(kk::kMaxSpeed, "Max Speed", 0, 100, 5, "%", &maxSpeed);
    b.Slider(kk::kMaxYaw, "Max Yaw", 0, 100, 5, "%", &maxYaw);
    b.Slider(kk::kDeadzone, "Deadzone", 0, 40, 5, "%", &deadzone);
    b.Slider(kk::kExpo, "Expo", 0, 100, 5, "%", &expo);
    b.Slider(kk::kMinPwm, "Min PWM", 0, 100, 5, "%", &minPwm);
    b.Select(kk::kMixMode, "Mix Mode", "Standard;Alt(4-axis)", &mixMode);
    b.Paragraph("", "Invert Axes");
    { sets::Row r(b); }
    b.Switch(kk::kInvertX, "X", &invertX);
    b.Switch(kk::kInvertY, "Y", &invertY);
    b.Switch(kk::kInvertR, "Rot", &invertR);
    b.Paragraph("", "Motor Reverse");
    { sets::Row r(b); }
    b.Switch(kk::kInvFrontLeft, "FrontLeft", &mFrontLeft.invert);
    b.Switch(kk::kInvFrontRight, "FrontRight", &mFrontRight.invert);
    b.Switch(kk::kInvRearLeft, "RearLeft", &mRearLeft.invert);
    b.Switch(kk::kInvRearRight, "RearRight", &mRearRight.invert);
    b.Paragraph("", "Motor Pins (valid: 13,14,16,17,18,19,21,22,23,25,26,27,32,33)");
    { sets::Row r(b); }
    b.Number(kk::kPinFrontLeftA, "FrontLeft A", &pinFrontLeftA, 0, 39);
    b.Number(kk::kPinFrontLeftB, "FrontLeft B", &pinFrontLeftB, 0, 39);
    { sets::Row r(b); }
    b.Number(kk::kPinFrontRightA, "FrontRight A", &pinFrontRightA, 0, 39);
    b.Number(kk::kPinFrontRightB, "FrontRight B", &pinFrontRightB, 0, 39);
    { sets::Row r(b); }
    b.Number(kk::kPinRearLeftA, "RearLeft A", &pinRearLeftA, 0, 39);
    b.Number(kk::kPinRearLeftB, "RearLeft B", &pinRearLeftB, 0, 39);
    { sets::Row r(b); }
    b.Number(kk::kPinRearRightA, "RearRight A", &pinRearRightA, 0, 39);
    b.Number(kk::kPinRearRightB, "RearRight B", &pinRearRightB, 0, 39);
    { sets::Row r(b); }
    if (b.Button("Apply Pins")) {
      if (!applyMotorPins()) {
        pinFrontLeftA = mFrontLeft.pinA; pinFrontLeftB = mFrontLeft.pinB;
        pinFrontRightA = mFrontRight.pinA; pinFrontRightB = mFrontRight.pinB;
        pinRearLeftA = mRearLeft.pinA; pinRearLeftB = mRearLeft.pinB;
        pinRearRightA = mRearRight.pinA; pinRearRightB = mRearRight.pinB;
      }
      sett.reload();
    }
    b.Paragraph("", "Motor Test");
    b.Select(kk::kTestMotor, "Motor", "FrontLeft;FrontRight;RearLeft;RearRight", &singleTestMotor);
    if (b.Button("Test Selected")) {
      motorsEnabled = true;
      singleTestActive = true;
      singleTestStep = 0;
      singleTestTimer = millis();
      sett.reload();
    }

    { sets::Row r(b); }
    if (b.Button("Test Motors")) {
      motorsEnabled = true;
      testActive = true;
      testStep = 0;
      testTimer = millis();
      sett.reload();
    }
    if (b.Button("Reset Defaults")) {
      resetRequested = true;
    }
    if (b.Button("Back")) {
      showSettings = false;
      sett.reload();
    }
  }
}

void setupMotors() {
  ledcSetupChannel(mFrontLeft.pinA, mFrontLeft.chanA);
  ledcSetupChannel(mFrontLeft.pinB, mFrontLeft.chanB);
  ledcSetupChannel(mFrontRight.pinA, mFrontRight.chanA);
  ledcSetupChannel(mFrontRight.pinB, mFrontRight.chanB);
  ledcSetupChannel(mRearLeft.pinA, mRearLeft.chanA);
  ledcSetupChannel(mRearLeft.pinB, mRearLeft.chanB);
  ledcSetupChannel(mRearRight.pinA, mRearRight.chanA);
  ledcSetupChannel(mRearRight.pinB, mRearRight.chanB);

  motorWrite(mFrontLeft, 0);
  motorWrite(mFrontRight, 0);
  motorWrite(mRearLeft, 0);
  motorWrite(mRearRight, 0);
}

void setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
}

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, STATUS_LED_ACTIVE_LOW ? HIGH : LOW);
#ifdef ESP32
  LittleFS.begin(true);
#else
  LittleFS.begin();
#endif

  db.begin();
  db.init(kk::kMotorsEnabled, motorsEnabled);
  db.init(kk::kMaxSpeed, maxSpeed);
  db.init(kk::kMaxYaw, maxYaw);
  db.init(kk::kDeadzone, deadzone);
  db.init(kk::kExpo, expo);
  db.init(kk::kMinPwm, minPwm);
  db.init(kk::kMixMode, mixMode);
  db.init(kk::kInvertX, invertX);
  db.init(kk::kInvertY, invertY);
  db.init(kk::kInvertR, invertR);
  db.init(kk::kInvFrontLeft, mFrontLeft.invert);
  db.init(kk::kInvFrontRight, mFrontRight.invert);
  db.init(kk::kInvRearLeft, mRearLeft.invert);
  db.init(kk::kInvRearRight, mRearRight.invert);
  db.init(kk::kPinFrontLeftA, pinFrontLeftA);
  db.init(kk::kPinFrontLeftB, pinFrontLeftB);
  db.init(kk::kPinFrontRightA, pinFrontRightA);
  db.init(kk::kPinFrontRightB, pinFrontRightB);
  db.init(kk::kPinRearLeftA, pinRearLeftA);
  db.init(kk::kPinRearLeftB, pinRearLeftB);
  db.init(kk::kPinRearRightA, pinRearRightA);
  db.init(kk::kPinRearRightB, pinRearRightB);
  db.init(kk::kTestMotor, singleTestMotor);

  // Load persisted values
  motorsEnabled = (bool)db[kk::kMotorsEnabled];
  maxSpeed = (uint8_t)db[kk::kMaxSpeed];
  maxYaw = (uint8_t)db[kk::kMaxYaw];
  deadzone = (uint8_t)db[kk::kDeadzone];
  expo = (uint8_t)db[kk::kExpo];
  minPwm = (uint8_t)db[kk::kMinPwm];
  mixMode = (uint8_t)db[kk::kMixMode];
  invertX = (bool)db[kk::kInvertX];
  invertY = (bool)db[kk::kInvertY];
  invertR = (bool)db[kk::kInvertR];
  mFrontLeft.invert = (bool)db[kk::kInvFrontLeft];
  mFrontRight.invert = (bool)db[kk::kInvFrontRight];
  mRearLeft.invert = (bool)db[kk::kInvRearLeft];
  mRearRight.invert = (bool)db[kk::kInvRearRight];
  pinFrontLeftA = (uint8_t)db[kk::kPinFrontLeftA];
  pinFrontLeftB = (uint8_t)db[kk::kPinFrontLeftB];
  pinFrontRightA = (uint8_t)db[kk::kPinFrontRightA];
  pinFrontRightB = (uint8_t)db[kk::kPinFrontRightB];
  pinRearLeftA = (uint8_t)db[kk::kPinRearLeftA];
  pinRearLeftB = (uint8_t)db[kk::kPinRearLeftB];
  pinRearRightA = (uint8_t)db[kk::kPinRearRightA];
  pinRearRightB = (uint8_t)db[kk::kPinRearRightB];
  singleTestMotor = (uint8_t)db[kk::kTestMotor];
  if (singleTestMotor > 3) singleTestMotor = 0;

  if (!applyMotorPins(true)) {
    pinFrontLeftA = 19; pinFrontLeftB = 21;
    pinFrontRightA = 26; pinFrontRightB = 27;
    pinRearLeftA = 33; pinRearLeftB = 32;
    pinRearRightA = 14; pinRearRightB = 13;
    applyMotorPins(true);
  }
  setupWiFi();

  sett.onBuild(buildUI);
  sett.onUpdate(onUpdate);
  sett.begin(true, "omni");
}

void loop() {
  sett.tick();
  db.tick();

  if (resetRequested) {
    resetRequested = false;
    db[kk::kMotorsEnabled] = motorsEnabled = true;
    db[kk::kMaxSpeed] = maxSpeed = 100;
    db[kk::kMaxYaw] = maxYaw = 100;
    db[kk::kDeadzone] = deadzone = 5;
    db[kk::kExpo] = expo = 20;
    db[kk::kMinPwm] = minPwm = 10;
    db[kk::kMixMode] = mixMode = 0;
    db[kk::kInvertX] = invertX = true;
    db[kk::kInvertY] = invertY = true;
    db[kk::kInvertR] = invertR = true;
    db[kk::kInvFrontLeft] = mFrontLeft.invert = false;
    db[kk::kInvFrontRight] = mFrontRight.invert = false;
    db[kk::kInvRearLeft] = mRearLeft.invert = false;
    db[kk::kInvRearRight] = mRearRight.invert = false;
    db[kk::kPinFrontLeftA] = pinFrontLeftA = 19;
    db[kk::kPinFrontLeftB] = pinFrontLeftB = 21;
    db[kk::kPinFrontRightA] = pinFrontRightA = 26;
    db[kk::kPinFrontRightB] = pinFrontRightB = 27;
    db[kk::kPinRearLeftA] = pinRearLeftA = 33;
    db[kk::kPinRearLeftB] = pinRearLeftB = 32;
    db[kk::kPinRearRightA] = pinRearRightA = 14;
    db[kk::kPinRearRightB] = pinRearRightB = 13;
    db[kk::kTestMotor] = singleTestMotor = 0;
    applyMotorPins();
    sett.reload();
  }

  bool clientActive = (millis() - lastClientMs) < CLIENT_TIMEOUT_MS;
  if (clientActive) {
    digitalWrite(STATUS_LED_PIN, (true ^ STATUS_LED_ACTIVE_LOW) ? HIGH : LOW);
  } else {
    static uint32_t blinkMs = 0;
    static bool blinkState = false;
    uint32_t now = millis();
    if (now - blinkMs >= LED_BLINK_MS) {
      blinkMs = now;
      blinkState = !blinkState;
    }
    digitalWrite(STATUS_LED_PIN, (blinkState ^ STATUS_LED_ACTIVE_LOW) ? HIGH : LOW);
  }

  if (testActive || singleTestActive) {
    runMotorTest();
    return;
  }

  // Joystick values are expected in -100..100 range.
  float x = (float)joyMove.x / 100.0f;
  float y = (float)joyMove.y / 100.0f;
  float rx = (float)joyRotate.x / 100.0f;
  float ry = (float)joyRotate.y / 100.0f;

  // Rotate joystick logic 90 degrees clockwise (phone horizontal)
  rotate90CW(x, y);
  rotate90CW(rx, ry);

  if (invertX) x = -x;
  if (invertY) y = -y;
  if (invertR) rx = -rx;

  float dz = deadzone / 100.0f;
  x = applyDeadzone(x, dz);
  y = applyDeadzone(y, dz);
  rx = applyDeadzone(rx, dz);
  ry = applyDeadzone(ry, dz);

  float e = expo / 100.0f;
  float spd = maxSpeed / 100.0f;
  float yaw = maxYaw / 100.0f;
  x = applyExpo(x, e) * spd;
  y = applyExpo(y, e) * spd;
  rx = applyExpo(rx, e) * yaw;
  ry = applyExpo(ry, e) * yaw;

  if (mixMode == 0) {
    mixAndDrive(x, y, rx);
  } else {
    mixAndDriveAlt(x, y, rx, ry);
  }
}
void onUpdate(sets::Updater& upd) {
  (void)upd;
  lastClientMs = millis();
}
