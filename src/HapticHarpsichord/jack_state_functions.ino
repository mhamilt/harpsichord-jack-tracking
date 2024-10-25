void updateJackStates() {

  tempStatePointer = prevStates;
  prevStates = jackStates;
  jackStates = tempStatePointer;

  for (int i = 0; i < numSensors; i++) {
    if (prevSensorReadings[i] <= releaseThresholds[i])
      jackStates[i] = RELEASED;
    else if (prevSensorReadings[i] < pluckThresholds[i] and currSensorReadings[i] > pluckThresholds[i])
      jackStates[i] = PLUCK;
    else if (prevSensorReadings[i] > pluckThresholds[i] and currSensorReadings[i] > pluckThresholds[i])
      jackStates[i] = PLUCKED;
  }

  // if (prevSensorReadings[0] > pluckThresholds[0] and currSensorReadings[0] < pluckThresholds[0])
  //   jackStates[0] = RELEASING;
  // if (prevSensorReadings[1] > pluckThresholds[1] and currSensorReadings[1] < pluckThresholds[1])
  //   jackStates[1] = RELEASING;
  // if (prevSensorReadings[2] > pluckThresholds[2] and currSensorReadings[2] < pluckThresholds[2])
  //   jackStates[2] = RELEASING;
}

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