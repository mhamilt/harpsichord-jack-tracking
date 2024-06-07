/*

 Flash Store and Retrieve:

 Example of reading and writing to flash memory using a rotary encoder

 Note: the area of flash memory reserved for the variable is
 lost every time the sketch is uploaded on the board.
*/

#define FLASH_DEBUG 0
#include <FlashStorage_SAMD.h>  // To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <Rotary.h>
#include <Button2.h>
//-----------------------------------------------------------------------------
const byte BUTTON_PIN = 10;
const byte ROTARY_PIN1 = 11;
const byte ROTARY_PIN2 = 12;
const byte CLICKS_PER_STEP = 4;
const byte MIN_POS = 0;
const byte MAX_POS = 2;
const byte START_POS = 0;
const byte INCREMENT = 1;
Rotary r = Rotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, MIN_POS, MAX_POS, START_POS, INCREMENT);
Button2 b = Button2(BUTTON_PIN);
const byte ledPins[3] = { 2, 3, 5 };
byte curKey = 0;
bool isKeySelectMode = true;
bool newData = true;
byte valToWrite = 0;
//-----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  delay(200);

  Serial.print(F("\nStart FlashStoreAndRetrieve on "));
  Serial.println(BOARD_NAME);
  Serial.println(FLASH_STORAGE_SAMD_VERSION);

  Serial.print("EEPROM length: ");
  Serial.println(EEPROM.length());

  r.setChangedHandler(rotate);
  b.setLongClickHandler(longclick);
  b.setDoubleClickHandler(doubleclick);
}

void loop() {
  r.loop();
  b.loop();
  updatePrint();
}

//-----------------------------------------------------------------------------
void rotate(Rotary& r) {
  valToWrite = r.getPosition();
  Serial.print(r.getPosition());  
}

void doubleclick(Button2& btn) {
  uint16_t address = 0;
  byte number0;
  byte number1;
  byte number2;

  // Read the content of emulated-EEPROM
  EEPROM.get(address + 0, number0);
  EEPROM.get(address + 1, number1);
  EEPROM.get(address + 2, number2);

  // Print the current number on the serial monitor
  Serial.print("\nNumbers: ");
  Serial.print(number0);
  Serial.print(" ");
  Serial.print(number1);
  Serial.print(" ");
  Serial.println(number2);
}

void longclick(Button2& btn) {
  uint16_t address = 0;

  EEPROM.put(address + 0, valToWrite);
  EEPROM.put(address + 1, valToWrite);
  EEPROM.put(address + 2, valToWrite);

  if (!EEPROM.getCommitASAP()) {
    Serial.println("CommitASAP not set. Need commit()");
    EEPROM.commit();
  }

  newData = true;
}
//-----------------------------------------------------------------------------

void updatePrint() {
  if (newData) {
    newData = false;

    uint16_t address = 0;
    byte number0;
    byte number1;
    byte number2;

    // Read the content of emulated-EEPROM
    EEPROM.get(address + 0, number0);
    EEPROM.get(address + 1, number1);
    EEPROM.get(address + 2, number2);

    // Print the current number on the serial monitor
    Serial.print("Numbers: ");
    Serial.print(number0);
    Serial.print(" ");
    Serial.print(number1);
    Serial.print(" ");
    Serial.println(number2);
  }
}

void printData() {
  uint16_t address = 0;
  byte number0;
  byte number1;
  byte number2;

  // Read the content of emulated-EEPROM
  EEPROM.get(address + 0, number0);
  EEPROM.get(address + 1, number1);
  EEPROM.get(address + 2, number2);

  // Print the current number on the serial monitor
  Serial.print("\nNumbers: ");
  Serial.print(number0);
  Serial.print(" ");
  Serial.print(number1);
  Serial.print(" ");
  Serial.println(number2);
}