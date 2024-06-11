/*
  Testing of Calibration (Debug) Mode for Jack Sensors

  Holding down the button of the rotary encoder on startup will enter debug mode.
  From here you can select which pluck threshold to modify by turning the rotary encoder then pressing the button
  
  A double click will exit debug mode and wrirte the new thresholds to the EEPROM.

  If the EEPROM has never been written to before, the default pluckValues will be written and used.

 */
#include <Rotary.h>
#include <Button2.h>
#include <FlashStorage_SAMD.h>
//-----------------------------------------------------------------------------
// Rotary Variables
const byte BUTTON_PIN = 10;
const byte ROTARY_PIN1 = 11;
const byte ROTARY_PIN2 = 12;
const byte CLICKS_PER_STEP = 4;
const byte MIN_POS = 0;
const byte MAX_POS = 2;
const byte START_POS = 0;
const byte INCREMENT = 1;
Rotary r = Rotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, MIN_POS, MAX_POS, START_POS, INCREMENT);
Button2 b = Button2(BUTTON_PIN);
const byte ledPins[3] = { 2, 3, 5 };
byte curKey = 0;
bool isKeySelectMode = true;
bool executeDebugMode = false;
//-----------------------------------------------------------------------------
// Sensor variables
const int sensorPins[3] = { A2, A1, A0 };
byte pluckValues[3] = { 158, 57, 61 };
unsigned long sensorHomeValues[3] = { 0, 0, 0 };
int sensorValues[3] = { 0, 0, 0 };

//-----------------------------------------------------------------------------
// EEPROM Vars
const uint16_t tagAddress = 0;
const char tagName[4] = { 'D', 'A', 'T', 'A' };
const uint16_t pluckValAddress = tagAddress + 4;
//-----------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  analogReference(AR_INTERNAL1V65);
  readPluckFromEEPROM();
  calibrate();

  if (b.isPressed()) {

    setupDebugMode();

    while (executeDebugMode)
      debugModeLoop();

    writePluckToEEPROM();

    for (auto pin : ledPins) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }
  }
}

void loop() {
  updateSensorReadings();
  printReadings();
}
//-----------------------------------------------------------------------------
// Debug functions

void setupDebugMode() {
  Serial.println("DEBUG MODE");
  executeDebugMode = true;
  r.setChangedHandler(rotate);
  b.setDoubleClickHandler(doubleclick);
  b.setClickHandler(click);

  for (auto pin : ledPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  digitalWrite(ledPins[r.getPosition()], HIGH);
}

void debugModeLoop() {
  r.loop();
  b.loop();
  updateSensorReadings();
  printReadings();
}

void exitDebug() {
  executeDebugMode = false;
}


//-----------------------------------------------------------------------------
// Rotary Callbacks

void click(Button2& btn) {
  isKeySelectMode = !isKeySelectMode;
  if (isKeySelectMode) {
    r.setUpperBound(2);
    r.setLowerBound(0);
    r.resetPosition(curKey, false);
  } else {
    r.setUpperBound(255);
    r.setLowerBound(0);
    r.resetPosition(pluckValues[curKey], false);
  }
}


void doubleclick(Button2& btn) {
  exitDebug();
}


void rotate(Rotary& r) {
  if (isKeySelectMode) {

    curKey = r.getPosition();

    for (auto pin : ledPins) {
      if (pin == ledPins[curKey]) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }
    }
  } else {
    pluckValues[curKey] = r.getPosition();
  }
}

//-----------------------------------------------------------------------------

void printReadings() {
  Serial.print("min:");
  Serial.print(0);
  Serial.print(' ');

  Serial.print("max:");
  Serial.print(255);
  Serial.print(' ');

  if (!isKeySelectMode and curKey == 0)
    Serial.print("(edit)");
  Serial.print("p1:");
  Serial.print(pluckValues[0]);
  Serial.print(' ');

  if (!isKeySelectMode and curKey == 1)
    Serial.print("(edit)");
  Serial.print("p2:");
  Serial.print(pluckValues[1]);
  Serial.print(' ');

  if (!isKeySelectMode and curKey == 2)
    Serial.print("(edit)");
  Serial.print("p3:");
  Serial.print(pluckValues[2]);
  Serial.print(' ');

  Serial.print("s1:");
  Serial.print(sensorValues[0]);
  Serial.print(' ');

  Serial.print("s2:");
  Serial.print(sensorValues[1]);
  Serial.print(' ');

  Serial.print("s3:");
  Serial.println(sensorValues[2]);
}


void printPluckValues() {
  Serial.print(pluckValues[0]);
  Serial.print(' ');
  Serial.print(pluckValues[1]);
  Serial.print(' ');
  Serial.print(pluckValues[2]);
}

//-----------------------------------------------------------------------------

void calibrate() {
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 3; i++) {
      sensorHomeValues[i] += analogRead(sensorPins[i]);
    }
  }

  for (int i = 0; i < 3; i++) {
    sensorHomeValues[i] >>= 3;
  }
}

void updateSensorReadings() {
  sensorValues[0] = map(analogRead(sensorPins[0]), sensorHomeValues[0], 130, 0, 255);
  sensorValues[1] = map(analogRead(sensorPins[1]), sensorHomeValues[1], 130, 0, 255);
  sensorValues[2] = map(analogRead(sensorPins[2]), sensorHomeValues[2], 130, 0, 255);
  sensorValues[0] = (sensorValues[0] < 0) ? 0 : sensorValues[0];
  sensorValues[1] = (sensorValues[1] < 0) ? 0 : sensorValues[1];
  sensorValues[2] = (sensorValues[2] < 0) ? 0 : sensorValues[2];
}

//-----------------------------------------------------------------------------
// EEPROM Functions

/// Read Data from EEPROM if the DATA tag is present in memory. If not, this
/// must be a first in which case use the default values for pluckValues and
/// write that to memory.
void readPluckFromEEPROM() {

  uint32_t tagRead;
  EEPROM.get<uint32_t>(tagAddress, tagRead);

  if (tagRead != *((uint32_t*)tagName)) {
    EEPROM.put<char[4]>(tagAddress, tagName);
    EEPROM.put<byte[3]>(pluckValAddress, pluckValues);

    if (!EEPROM.getCommitASAP())
      EEPROM.commit();

  } else {
    EEPROM.get<byte[3]>(pluckValAddress, pluckValues);
  }
}

void writePluckToEEPROM() {

  EEPROM.put<byte[3]>(pluckValAddress, pluckValues);

  if (!EEPROM.getCommitASAP())
    EEPROM.commit();
}