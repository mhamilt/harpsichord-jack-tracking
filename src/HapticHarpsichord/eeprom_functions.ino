//-----------------------------------------------------------------------------
// EEPROM Functions

/// Read Data from EEPROM if the DATA tag is present in memory. If not, this
/// must be a first in which case use the default values for pluckValues and
/// write that to memory.
void readPluckFromEEPROM() {

  uint32_t tagRead;

  fram.read(tagAddress, (uint8_t*)&tagRead, 4);

  if (tagRead != *((uint32_t*)tagName)) {
    fram.write(tagAddress, tagName, 4);
    fram.write(pluckValAddress, (uint8_t*)pluckThresholds, numSensors * 4);
  } else {
    fram.read(pluckValAddress, (uint8_t*)pluckThresholds, numSensors * 4);
  }
  
}

void writePluckToEEPROM() {
  fram.write(pluckValAddress, (uint8_t*)pluckThresholds, numSensors * 4);
}
// -----------------------------------------------------------------------------