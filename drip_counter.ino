#include <JeeLib.h>
#include <Wire.h>
#include "RTClib.h"

const int knockSensor = 2; 

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup for low power waiting, though I don't actually do any

RTC_DS1307 RTC;

void setup() {
  pinMode(2, INPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
  Wire.begin();
  RTC.begin();

  if (!RTC.isrunning()) {
    Serial.println("No RTC soz, might mean it's not running so gonna adjust it");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  Serial.println("I'm gonna sleep");
  //Have to delay before going to sleep, otherwise it cuts off the serial printing
  delay(100);
  
  attachInterrupt(digitalPinToInterrupt(knockSensor), piezoEvent, RISING);
  
  Sleepy::powerDown();
  
  Serial.println("woke up!");
  
  DateTime now = RTC.now();
  Serial.println(now.unixtime());
  delay(100);
}

void piezoEvent() {
  detachInterrupt(digitalPinToInterrupt(knockSensor));
}
