//-----------------------------------------------------------------------------
// Rotary Callbacks

void click(Button2& btn) {
  isKeySelectMode = !isKeySelectMode;
  if (isKeySelectMode) {
    rotary.setUpperBound(numSensors);
    rotary.setLowerBound(0);
    rotary.resetPosition(curKey, false);
  } else {
    rotary.setUpperBound(1024);
    rotary.setLowerBound(0);
    rotary.resetPosition(pluckThresholds[curKey], false);
  }
}


void doubleclick(Button2& btn) {
  executeDebugMode = false;
}


void rotate(Rotary& r) {
  if (isKeySelectMode) {
    curKey = r.getPosition();
    
    leds.clear();
    leds.setPixelColor(curKey, 100, 0, 0);
    leds.show();
    } else {
      pluckThresholds[curKey] = r.getPosition();
  }
}
