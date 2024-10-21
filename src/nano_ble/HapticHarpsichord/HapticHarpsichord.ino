/*
  Haptic Harposchord

  This is the full firmware for the haptic harpsochord using the arduino IDE development platform

  At time of writing the microcontroller for the project is not fixed. As such there may be macros to help make the correct changes to firmware behaviour.


 */
#define numSensors 49
#define numMuxChannels 7
#define numPcbs 7

#if (numMuxChannels * numPcbs != numSensors)
#error "Check product of mux channels and number of PCBs is equal to total number of sensors"
#endif

const unsigned long midiBaudRate = 31250;

const size_t muxPinA = 6;
const size_t muxPinB = 7;
const size_t muxPinC = 8;
// const size_t muxPinD = 9;

int sensorReadingsA[64];
int sensorReadingsB[64];
int* prevSensorReadings = sensorReadingsA;
int* currSensorReadings = sensorReadingsA;
int* tempPointer;
int sensorAvgMaxima[64];
int sensorAvgMinima[64];
int sensorThreshold[64];
uint32_t readCount = 0;
uint64_t lastRead = 0;

void setup() {

  Serial.begin(midiBaudRate);

  setOperationMode();

  if (false /*debug mode*/)
    debugLoop();

  /// setup pin
  pinMode(muxPinA, OUTPUT);
  pinMode(muxPinB, OUTPUT);
  pinMode(muxPinC, OUTPUT);
  /// init LEDs
  /// setup EEPROM
  /// calibrarte sensors
}

void loop() {

  readSensors();
  readCount++;

  if (readCount > 2048) {
    readCount = 0;
    uint64_t now = millis();
    Serial.println(now - lastRead);
    lastRead = now;
  }
  // updateJackStates();
  // printReadings();
  // checkJackStates();
  // // printJackStates();
}


/// Set if the device will operate in a standard or debug mode
void setOperationMode() {
}

/// Read all sensors and put results in currSensorReadings
void readSensors() {

  // pointer swap
  tempPointer = prevSensorReadings;
  prevSensorReadings = currSensorReadings;
  currSensorReadings = tempPointer;

  for (int mux = 0; mux < numMuxChannels; mux++) {

    // switch mux channel
    digitalWrite(muxPinA, (mux >> 0) & 0x1);
    digitalWrite(muxPinB, (mux >> 1) & 0x1);
    digitalWrite(muxPinC, (mux >> 2) & 0x1);
    // digitalWrite(muxPinD, (mux >> 3) & 0x1);

    for (int adc = 0; adc < numPcbs; adc++) {

      int i = mux + (adc * numPcbs);
      currSensorReadings[i] = analogRead(A0 + adc);
    }
  }
}

/// Read a single sensor and return its raw value
int readSensor(byte index) {
  int adc = index / numPcbs;
  int mux = index - adc * numPcbs;

  digitalWrite(muxPinA, (mux >> 0) & 0x1);
  digitalWrite(muxPinB, (mux >> 1) & 0x1);
  digitalWrite(muxPinC, (mux >> 2) & 0x1);
  // digitalWrite(muxPinD, (mux >> 3) & 0x1);

  return analogRead(A0 + adc);
}

void calibrateSensors() {
  setThresholdsFromEEPROM();
  // readSensors();
  setAverages();
}


void setThresholdsFromEEPROM() {
}

void setAverages() {
}
