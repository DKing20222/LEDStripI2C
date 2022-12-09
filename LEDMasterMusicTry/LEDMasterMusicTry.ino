#include <FastLED.h>
#include <Wire.h>

//I2C
#define SlaveAddress 1

//LED
#define NUM_LEDS 240

//Ultrasonic sensor
#define echoPin 9
#define trigPin 10
long duration;
int distance;
byte ModeSwitchDistance = 30;

//Sound sensor
#define AudioPin 8
#define AnalogRead 0
bool SoundIO = 0;

void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(AudioPin, INPUT);
  pinMode(AnalogRead, INPUT);
  Wire.begin();
  Serial.begin(9600);
}
void loop()
{
  Music();
  EVERY_N_SECONDS(3) {
    Ultrasonic;
  }
}

void Ultrasonic() {
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if ((distance < ModeSwitchDistance) && (distance > 0)) {
    //LEDMode += 1;
    Wire.beginTransmission(SlaveAddress);
    Wire.write('1'); //LEDMode
    Wire.endTransmission();
  }
}
void Music(){
  float Tone = analogRead(AnalogRead);
  //Serial.println(Tone);
  int LEDMap = map(Tone, 0, 1023, 0, NUM_LEDS);
  if (LEDMap < 30){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('a');
    Serial.println('a');
    Wire.endTransmission();
  }
  /*else if ((30 <= LEDMap)&& (LEDMap < 60)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('b');
    Wire.endTransmission();
  }
  else if ((30 <= LEDMap)&& (LEDMap < 60)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('c');
    Wire.endTransmission();
  }
  else if ((60 <= LEDMap)&& (LEDMap < 90)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('d');
    Wire.endTransmission();
  }
  else if ((90 <= LEDMap)&& (LEDMap < 120)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('e');
    Wire.endTransmission();
  }
  else if ((120 <= LEDMap)&& (LEDMap < 150)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('f');
    Wire.endTransmission();
  }
  else if ((150 <= LEDMap)&& (LEDMap < 180)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('g');
    Wire.endTransmission();
  }
  else if ((180 <= LEDMap)&& (LEDMap < 210)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('h');
    Wire.endTransmission();
  }
  else if ((210 <= LEDMap)&& (LEDMap < 240)){
    Wire.beginTransmission(SlaveAddress);
    Wire.write('i');
    Wire.endTransmission();
  }*/
}
