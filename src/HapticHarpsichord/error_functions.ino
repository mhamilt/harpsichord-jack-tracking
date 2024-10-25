void blinkRGB(size_t ledPin, unsigned long duration, unsigned long pause = 0, byte count = 1) {

  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, LOW);
    delay(duration);
    digitalWrite(ledPin, HIGH);
    delay(duration);
  }

  delay(pause);
}

void halt(HapticHarpsichordErrorCodes errorCode) {

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  switch (errorCode) {
    case FRAM_NOT_FOUND:
      while (true) {
        blinkRGB(LEDB, 500, 2000, 3);
      }
      break;
    default:
      break;
  }
}
