// void printJackStates() {
//   bool anyPrint = false;

//   if (jackStates[0] == PLUCK and prevStates[0] == RELEASED) {
//     Serial.print("Jack 1 Plucked ");
//     anyPrint = true;
//   }
//   if (jackStates[1] == PLUCK and prevStates[1] == RELEASED) {
//     Serial.print("Jack 2 Plucked ");
//     anyPrint = true;
//   }
//   if (jackStates[2] == PLUCK and prevStates[2] == RELEASED) {
//     Serial.print("Jack 3 Plucked ");
//     anyPrint = true;
//   }
//   if (anyPrint)
//     Serial.println();
// }

//-----------------------------------------------------------------------------

void printJackReading(byte index) {
  Serial.print("min:");
  Serial.print(0);
  Serial.print(' ');

  Serial.print("max:");
  Serial.print(255);
  Serial.print(' ');

  Serial.print("Jack");
  Serial.print(index);
  Serial.print(":");
  Serial.print(readSensor(index));
  Serial.print(' ');
}

void printJackThreshold(byte index)
{
  Serial.print("Thresh");
  Serial.print(index);
  Serial.print(":");
  Serial.print(sensorThreshold[index]);
}