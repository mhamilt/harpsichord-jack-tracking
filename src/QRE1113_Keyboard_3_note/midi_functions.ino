//-----------------------------------------------------------------------------
// MIDI Functions


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { NOTE_OFF, (NOTE_OFF << 4) | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

void aftertouch(byte channel, byte key, byte pressure) {
  midiEventPacket_t event = { 0xA, 0xA0 | channel, key, pressure };
  MidiUSB.sendMIDI(event);
}

void sendMidiEvent(MIDI_EVENT_STATUS status, byte channel, byte val1, byte val2) {
  midiEventPacket_t event = { status, (status << 4) | channel, val1, val2 };
  MidiUSB.sendMIDI(event);
}
//-----------------------------------------------------------------------------
