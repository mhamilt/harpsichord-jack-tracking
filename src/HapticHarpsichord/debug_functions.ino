//-----------------------------------------------------------------------------
// Debug functions

void setupDebugMode() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, LOW);
  executeDebugMode = true;
  rotary.setChangedHandler(rotate);
  button.setDoubleClickHandler(doubleclick);
  button.setClickHandler(click);

  // leds.clear();
  // leds.fill(leds.Color(1, 1, 0), 0, numSensors);
  // leds.setPixelColor(key2index(rotary.getPosition()), 0, 0, 100);
  // leds.show();
}


/// run loop for debug mode
void debugLoop() {
  setupDebugMode();
  
  curKeyIndex = rotary.getPosition();
  leds.fill(leds.Color(0, 0, 0), 0, numSensors);
  leds.setPixelColor(curKeyIndex, 0, 0, 255);
  leds.show();

  unsigned long now = millis();

  while (executeDebugMode) {

    setCurrentKey();
    rotary.loop();
    button.loop();
    readSensors();

    for (int i = 0; i < numSensors; i++) {
      // if (index2key(i) >= 20 and index2key(i) <= 30) {
      if (currSensorReadings[i] < pluckThresholds[i] and prevSensorReadings[i] > pluckThresholds[i]) {
        noteOff(0, index2note(i), 100);
        // if (i == curKeyIndex)
        //   leds.setPixelColor(i, 50, 0, 0);
        //   leds.show();

      } else if (currSensorReadings[i] > pluckThresholds[i] and prevSensorReadings[i] < pluckThresholds[i]) {
        // leds.setPixelColor(i, 0, 100, 0);
        noteOn(0, index2note(i), 100);
        // leds.show();
      }
      // }
    }
    printJackReading(curKeyIndex);
    printJackThreshold(curKeyIndex);
    Serial.println();

    if (currSensorReadings[curKeyIndex] < sensorAvgMinima[curKeyIndex]) {
      sensorAvgMinima[curKeyIndex] = currSensorReadings[curKeyIndex];
    }
  }

  exitDebug();
}

void setCurrentKey() {
  for (int i = 0; i < numSensors; i++) {
    // if (index2key(i) >= 20 and index2key(i) <= 30) {
    // if (readSensor(i) < 500 and i != curKeyIndex) {
    // curKeyIndex = i;
    // leds.fill(leds.Color(0, 0, 0), 0, numSensors);
    // leds.setPixelColor(curKeyIndex, 0, 0, 100);
    // leds.show();
    // }
    // }
  }
}

void exitDebug() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, LOW);
  writePluckToEEPROM();

  leds.clear();
}
