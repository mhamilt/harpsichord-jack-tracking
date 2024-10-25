//-----------------------------------------------------------------------------
// Debug functions

void setupDebugMode() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, LOW);
  executeDebugMode = true;
  rotary.setChangedHandler(rotate);
  button.setDoubleClickHandler(doubleclick);
  button.setClickHandler(click);

  leds.clear();
  leds.setPixelColor(rotary.getPosition(), 100, 0, 0);
  leds.show();
}

/// run loop for debug mode
void debugLoop() {

  /// select sensor
  /// adjust thresholds
  /// write too eeprom
  if (button.isPressed()) {

    setupDebugMode();

    while (executeDebugMode)
      debugModeLoop();

    exitDebug();
  }
}

void debugModeLoop() {
  rotary.loop();
  button.loop();
  readSensors();
  if (!isKeySelectMode) {
    printJackReading(curKey);
    printJackThreshold(curKey);
    Serial.println();
  }
}


void exitDebug() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDB, LOW);
  writePluckToEEPROM();

  leds.clear();
}
