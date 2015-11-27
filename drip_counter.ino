#include <JeeLib.h>

const int knockSensor = 2; 

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup for low power waiting, though I don't actually do any

void setup() {
  pinMode(2, INPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
}

void loop() {
  Serial.println("I'm gonna sleep");
  //Have to delay before going to sleep, otherwise it cuts off the serial printing
  delay(100);
  
  attachInterrupt(digitalPinToInterrupt(knockSensor), piezoEvent, RISING);
  Sleepy::powerDown();
  
  Serial.println("woke up!");
}

void piezoEvent() {
  detachInterrupt(digitalPinToInterrupt(knockSensor));
}
