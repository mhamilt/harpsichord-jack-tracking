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

  while (executeDebugMode) {

    if (millis() - now > 10) {
      rainbow(step++);
      now = millis();
    }

    // setCurrentKey();
    readSensors();
    rotary.loop();
    button.loop();
    
    for (int i = 0; i < numSensors; i++) {
      if (currSensorReadings[i] < pluckThresholds[i] and prevSensorReadings[i] > pluckThresholds[i]) {
        noteOff(0, index2note(i), 100);
      } else if (currSensorReadings[i] > pluckThresholds[i] and prevSensorReadings[i] < pluckThresholds[i]) {
        noteOn(0, index2note(i), 100);
      }
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

void setCurrentKey(constexpr thresh) {
  for (int i = 0; i < numSensors; i++) {
    if (readSensor(i) < thresh and i != curKeyIndex) {
      curKeyIndex = i;
      leds.fill(leds.Color(0, 0, 0), 0, numSensors);
      leds.setPixelColor(curKeyIndex, 0, 0, 100);
      leds.show();
    }
  }
}

void exitDebug() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, LOW);
  writePluckToEEPROM();

  leds.clear();
}
