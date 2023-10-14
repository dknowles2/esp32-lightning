#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#define RXD2 16
#define TXD2 17

DFRobotDFPlayerMini mp3;

const int kAudioHigh = 3000;
const int kAudioLow = 2000;
const int kPinAudioBusy = 4;  // Audio player busy
const int kPinAudioLevel = 2; // Audio level samples
const int kPinLights = 12;    // IRL520 mosfet gate for LED strip light

void setup()
{
  pinMode(kPinLights, OUTPUT);
  pinMode(kPinAudioLevel, OUTPUT);
  pinMode(kPinAudioBusy, INPUT);

  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);

  while (!mp3.begin(Serial1, /*isACK=*/true, /*doReset=*/true))
  {
    Serial.println();
    Serial.println("Failed to initialize DFPlayer");
    delay(1000);
  }

  mp3.volume(30);
}

void loop()
{
  Serial.println("");
  mp3.play(1);
  delay(100);

  for (int audioBusy = digitalRead(kPinAudioBusy); audioBusy == 0;)
  {
    int audioLevel = analogRead(kPinAudioLevel);
    Serial.print(audioLevel);
    Serial.print(" ");
    if (audioLevel >= kAudioHigh)
    {
      Serial.println("large number! ");
      digitalWrite(kPinLights, HIGH);
    }
    if (audioLevel <= kAudioLow)
    {
      digitalWrite(kPinLights, LOW);
    }
  }
}
