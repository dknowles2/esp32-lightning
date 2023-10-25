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

bool audioPlaying()
{
  return digitalRead(kPinAudioBusy) == 0;
}

void playThunder()
{
  Serial.print("Playing thunder MP3");
  mp3.play(1);
  while (!audioPlaying())
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
}

void loop()
{
  playThunder();
  bool ledOn = false;
  while (audioPlaying())
  {
    int audioLevel = analogRead(kPinAudioLevel);
    if (!ledOn && audioLevel >= kAudioHigh)
    {
      ledOn = true;
      digitalWrite(kPinLights, HIGH);
      Serial.print("*");
    }
    if (ledOn && audioLevel <= kAudioLow)
    {
      ledOn = false;
      digitalWrite(kPinLights, LOW);
    }
  }
  Serial.println();
}
