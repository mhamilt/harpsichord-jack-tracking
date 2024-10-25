/*
  Haptic Harposchord

  This is the full firmware for the haptic harpsochord using the arduino IDE development platform

  At time of writing the microcontroller for the project is not fixed. As such there may be macros to help make the correct changes to firmware behaviour.


 */
//-----------------------------------------------------------------------------
// Board specific libraries
#ifdef ARDUINO_ARDUINO_NANO33BLE
#include <PluggableUSBMIDI.h>
#elif ESP32
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>  // verision 3.1.3
#include <MIDI.h>
#elif ARDUINO_ARCH_STM32
#endif
//-----------------------------------------------------------------------------
// Board Agnostics Libraries
#include <Rotary.h>
#include <Button2.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_FRAM_SPI.h>
#include <SPI.h>
//-----------------------------------------------------------------------------
// Local Headers
#include "error_codes.h"
//-----------------------------------------------------------------------------
#define numSensors 49
#define numMuxChannels 7
#define numPcbs 7

#if (numMuxChannels * numPcbs != numSensors)
#error "Check product of mux channels and number of PCBs is equal to total number of sensors"
#endif
//-----------------------------------------------------------------------------
// Config Variables
const unsigned long midiBaudRate = 31250;
bool executeDebugMode = false;
//-----------------------------------------------------------------------------
// Operation Mode Variables
bool isKeySelectMode = true;
uint8_t curKey = 0;
//-----------------------------------------------------------------------------
// Multiplexer Variables
const size_t muxPinA = 6;
const size_t muxPinB = 7;
const size_t muxPinC = 8;
// const size_t muxPinD = 9;
//-----------------------------------------------------------------------------
// Sensor variables
uint32_t sensorReadingsA[numSensors];
uint32_t sensorReadingsB[numSensors];
uint32_t* prevSensorReadings = sensorReadingsA;
uint32_t* currSensorReadings = sensorReadingsA;
uint32_t* tempPointer;
uint32_t sensorAvgMaxima[numSensors];
uint32_t sensorAvgMinima[numSensors];
uint32_t pluckThresholds[numSensors];
uint32_t readCount = 0;
uint64_t lastRead = 0;
//-----------------------------------------------------------------------------
// LED Variables
const size_t ledPin = 9;
Adafruit_NeoPixel leds(numSensors, ledPin, NEO_GRB + NEO_KHZ800);
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
Rotary rotary = Rotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, MIN_POS, MAX_POS, START_POS, INCREMENT);
Button2 button = Button2(BUTTON_PIN);
//-----------------------------------------------------------------------------
// EEPROM Variables
uint8_t FRAM_CS = 13;
uint8_t FRAM_MISO = 4;
uint8_t FRAM_MOSI = 3;
uint8_t FRAM_SCK = 2;
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);
const uint32_t tagAddress = 0;
const uint8_t tagName[4] = { 'D', 'A', 'T', 'A' };
const uint32_t pluckValAddress = tagAddress + 4;
//-----------------------------------------------------------------------------
// MIDI Variables
USBMIDI MidiUSB;

void setup() {

  Serial.begin(midiBaudRate);

  /// setup mux
  pinMode(muxPinA, OUTPUT);
  pinMode(muxPinB, OUTPUT);
  pinMode(muxPinC, OUTPUT);

  /// init LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  leds.begin();
  leds.clear();

  /// setup EEPROM
  if (!fram.begin())
    halt(FRAM_NOT_FOUND);

  /// calibrarte sensors
  setOperationMode();

  if (false /*debug mode*/) {
    debugLoop();
  }
  
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
  setLedsToJackDisplacement();
  // updateJackStates();
  // printReadings();
  // checkJackStates();
  // // printJackStates();
}


/// Set if the device will operate in a standard or debug mode
void setOperationMode() {
}

