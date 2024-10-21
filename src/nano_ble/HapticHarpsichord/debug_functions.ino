// //-----------------------------------------------------------------------------
// // Debug functions

// void setupDebugMode() {
//   Serial.println("DEBUG MODE");
//   executeDebugMode = true;
//   r.setChangedHandler(rotate);
//   b.setDoubleClickHandler(doubleclick);
//   b.setClickHandler(click);

//   for (auto pin : ledPins) {
//     pinMode(pin, OUTPUT);
//     digitalWrite(pin, LOW);
//   }
//   digitalWrite(ledPins[r.getPosition()], HIGH);
// }

/// run loop for debug mode
void debugLoop() {

  /// select sensor
  /// adjust thresholds
  /// write too eeprom
  /*if (b.isPressed()) {

    setupDebugMode();

    while (executeDebugMode)
      debugModeLoop();

    exitDebug();
  }
  */
}

// void debugModeLoop() {
//   r.loop();
//   b.loop();
//   updateSensorReadings();
//   printReadings();
// }


// void exitDebug() {
//   writePluckToEEPROM();

//   for (auto pin : ledPins) {
//     pinMode(pin, OUTPUT);
//     digitalWrite(pin, LOW);
//   }
// }
