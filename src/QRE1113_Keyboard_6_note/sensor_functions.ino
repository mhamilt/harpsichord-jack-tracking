//-----------------------------------------------------------------------------

void calibrate() {
  Serial.print("Calibrating.... ");

  for (int j = 0; j < 8; j++)
    for (int i = 0; i < numSensors; i++)
      sensorHomeValues[i] += analogRead(sensorPins[i]);

  for (int i = 0; i < numSensors; i++)
    sensorHomeValues[i] >>= 3;

  Serial.print("Done!");
}

//-----------------------------------------------------------------------------

void updateSensorReadings() {
  for (int i = 0; i < numSensors; i++) {
    prevValues[i] = sensorValues[i];
    sensorValues[i] = map(analogRead(sensorPins[i]), sensorHomeValues[i], 130, 0, 255);
    sensorValues[i] = (sensorValues[i] < 0) ? 0 : sensorValues[i];
  }
}

//-----------------------------------------------------------------------------

void updateJackStates() {

  for (int i = 0; i < numSensors; i++) {
    prevStates[i] = jackStates[i];
    if (prevValues[i] <= releaseValues[i])
    {
      
      jackStates[i] = RELEASED;
    }
    else if (prevValues[i] < pluckValues[i] and sensorValues[i] > pluckValues[i])
    {
      
      jackStates[i] = PLUCK;
    }
    else if (prevValues[i] > pluckValues[i] and sensorValues[i] > pluckValues[i])
    {
      jackStates[i] = PLUCKED;
    }
  }
}

//-----------------------------------------------------------------------------

void checkJackStates() {
  bool anyTrigger = false;

  for (int i = 0; i < numSensors; i++) {
    if (jackStates[i] == PLUCK and prevStates[i] == RELEASED) {
      byte velocity = sensorValues[i] - prevValues[i];
      Serial.print("velocity: ");
      Serial.println(velocity);
      noteOn(0, 70 + i, velocity);
      if (ledFeedback) {
        digitalWrite(ledPins[i], HIGH);
      }

    } else if (jackStates[i] == RELEASED and prevStates[i] != RELEASED) {
      byte velocity = prevValues[i] - sensorValues[i];
      noteOff(0, 70+i, velocity);
      if (ledFeedback) {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}

byte getVelocity() {
  // value before
  // value after
  // millis in between
  millis();
  return 0;
}
