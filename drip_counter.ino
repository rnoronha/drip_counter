#include <JeeLib.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>

const int knockSensor = 2; 

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup for low power waiting, though I don't actually do any

RTC_DS1307 RTC;

int eeprom_addr = 0;

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
  unsigned long start = micros();
  DateTime now = RTC.now();
  unsigned long finish = micros();
  Serial.println(now.unixtime());
  Serial.println(finish - start);
  Serial.println(eeprom_addr);
  EEPROM.write(eeprom_addr, now.unixtime());
  eeprom_addr++;
  
  if(eeprom_addr % 4 == 0) {
    Serial.println("eeprom dump: ");
    for(int i = eeprom_addr - 4; i < eeprom_addr; i++) {
      byte value = EEPROM.read(i);
      Serial.print(i);
      Serial.print("\t");
      Serial.print(value, DEC);
      Serial.println();
    }
  }
}

void piezoEvent() {
  detachInterrupt(digitalPinToInterrupt(knockSensor));
}
