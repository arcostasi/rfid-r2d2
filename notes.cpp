#include <Arduino.h>
#include "notes.h"

void newTone(int speakerPin, float noteFrequency, long noteDuration)
{
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000 / noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000 / (microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x = 0; x < loopTime; x++)
  {
      digitalWrite(speakerPin,HIGH);
      delayMicroseconds(microsecondsPerWave);
      digitalWrite(speakerPin,LOW);
      delayMicroseconds(microsecondsPerWave);
  }
}
