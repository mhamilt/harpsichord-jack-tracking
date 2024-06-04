/*
  QRE1113  3-Note Keyboard
  
*/
//-----------------------------------------------------------------------------

#include <MIDIUSB.h>

//-----------------------------------------------------------------------------

enum JackState {
  PLUCKED,
  PLUCK,
  PRESSING,
  RELEASING,
  RELEASED,
  UNKNOWN_KEY_STATE
};

//-----------------------------------------------------------------------------

unsigned long sensorAverages[3] = { 0, 0, 0 };
unsigned long sensorHomeValues[3] = { 0, 0, 0 };
const int sensorPins[3] = { A2, A1, A0 };
int sensorValues[3] = { 0, 0, 0 };
int prevValues[3] = { 0, 0, 0 };
byte pluckValues[3] = { 150, 80, 90 };
byte releaseValues[3] = { 24, 24, 24 };
unsigned long currentTime[3]  = { 0, 0, 0 };
unsigned long previousTime[3] = { 0, 0, 0 };
JackState jackStates[3];
JackState prevStates[3];

//-----------------------------------------------------------------------------

void setup() {
  // analogReference(AR_DEFAULT);
  // analogReference(AR_INTERNAL);
  analogReference(AR_INTERNAL1V65);
  // analogReference(AR_INTERNAL1V0);
  Serial.begin(9600);
  Serial.print("Calibrating.... ");
  calibrate();
  Serial.print("Done!");
}

void loop() {
  updateSensorReadings();
  updateJackStates();
  printReadings();
  checkJackStates();
  // printJackStates();
  delay(1);
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

//-----------------------------------------------------------------------------

void updateSensorReadings() {
  prevValues[0] = sensorValues[0];
  prevValues[1] = sensorValues[1];
  prevValues[2] = sensorValues[2];
  sensorValues[0] = map(analogRead(sensorPins[0]), sensorHomeValues[0], 130, 0, 255);
  sensorValues[1] = map(analogRead(sensorPins[1]), sensorHomeValues[1], 130, 0, 255);
  sensorValues[2] = map(analogRead(sensorPins[2]), sensorHomeValues[2], 130, 0, 255);
  sensorValues[0] = (sensorValues[0] < 0) ? 0 : sensorValues[0];
  sensorValues[1] = (sensorValues[1] < 0) ? 0 : sensorValues[1];
  sensorValues[2] = (sensorValues[2] < 0) ? 0 : sensorValues[2];
  // sensorValues[0] = analogRead(sensorPins[0]);
  // sensorValues[1] = analogRead(sensorPins[1]);
  // sensorValues[2] = analogRead(sensorPins[2]);
}

//-----------------------------------------------------------------------------

void updateJackStates() {

  prevStates[0] = jackStates[0];
  prevStates[1] = jackStates[1];
  prevStates[2] = jackStates[2];


  if (prevValues[0] <= releaseValues[0])
    jackStates[0] = RELEASED;
  if (prevValues[1] <= releaseValues[1])
    jackStates[1] = RELEASED;
  if (prevValues[2] <= releaseValues[2])
    jackStates[2] = RELEASED;

  if (prevValues[0] < pluckValues[0] and sensorValues[0] > pluckValues[0])
    jackStates[0] = PLUCK;
  if (prevValues[1] < pluckValues[1] and sensorValues[1] > pluckValues[1])
    jackStates[1] = PLUCK;
  if (prevValues[2] < pluckValues[2] and sensorValues[2] > pluckValues[2])
    jackStates[2] = PLUCK;

  if (prevValues[0] > pluckValues[0] and sensorValues[0] > pluckValues[0])
    jackStates[0] = PLUCKED;
  if (prevValues[1] > pluckValues[1] and sensorValues[1] > pluckValues[1])
    jackStates[1] = PLUCKED;
  if (prevValues[2] > pluckValues[2] and sensorValues[2] > pluckValues[2])
    jackStates[2] = PLUCKED;

  // if (prevValues[0] > pluckValues[0] and sensorValues[0] < pluckValues[0])
  //   jackStates[0] = RELEASING;
  // if (prevValues[1] > pluckValues[1] and sensorValues[1] < pluckValues[1])
  //   jackStates[1] = RELEASING;
  // if (prevValues[2] > pluckValues[2] and sensorValues[2] < pluckValues[2])
  //   jackStates[2] = RELEASING;
}


//-----------------------------------------------------------------------------

void printReadings() {
  Serial.print(0);
  Serial.print(' ');
  Serial.print(255);
  Serial.print(' ');

  // Serial.print(sensorHomeValues[0]);
  // Serial.print(' ');
  // Serial.print(sensorHomeValues[1]);
  // Serial.print(' ');
  // Serial.print(sensorHomeValues[2]);
  // Serial.print(' ');


  Serial.print(sensorValues[0]);
  Serial.print(' ');
  Serial.print(sensorValues[1]);
  Serial.print(' ');
  Serial.println(sensorValues[2]);
}

//-----------------------------------------------------------------------------

void checkJackStates() {
  bool anyTrigger = false;
  if (jackStates[0] == PLUCK and prevStates[0] == RELEASED) {
    noteOn(0, 70, 127);
    anyTrigger = true;
  }
  if (jackStates[1] == PLUCK and prevStates[1] == RELEASED) {
    noteOn(0, 67, 127);
    anyTrigger = true;
  }
  if (jackStates[2] == PLUCK and prevStates[2] == RELEASED) {
    noteOn(0, 64, 127);
    anyTrigger = true;
  }
  if (jackStates[0] == RELEASED and prevStates[0] != RELEASED) {
    noteOff(0, 70, 127);
  }
  if (jackStates[1] == RELEASED and prevStates[1] != RELEASED) {
    noteOff(0, 67, 127);
  }
  if (jackStates[2] == RELEASED and prevStates[2] != RELEASED) {
    noteOff(0, 64, 127);
  }


  if (anyTrigger)
    ;
}

void printJackStates() {
  bool anyPrint = false;

  if (jackStates[0] == PLUCK and prevStates[0] == RELEASED) {
    Serial.print("Jack 1 Plucked ");
    anyPrint = true;
  }
  if (jackStates[1] == PLUCK and prevStates[1] == RELEASED) {
    Serial.print("Jack 2 Plucked ");
    anyPrint = true;
  }
  if (jackStates[2] == PLUCK and prevStates[2] == RELEASED) {
    Serial.print("Jack 3 Plucked ");
    anyPrint = true;
  }
  if (anyPrint)
    Serial.println();
}

//-----------------------------------------------------------------------------

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

//-----------------------------------------------------------------------------
byte getVelocity()
{
  // value before
  // value after
  // millis in between
  millis();
  return 0;
}
//-----------------------------------------------------------------------------