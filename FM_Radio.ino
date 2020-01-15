#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <AnalogSmooth.h>
#include <TEA5767N.h>
#include <LiquidCrystal.h>

int contrast = 120;
TEA5767N radio = TEA5767N();
int analogPin = 0;
int analogPin2 = 1;
int val = 0;
int frequencyInt = 0;
float frequency = 104.5;
float previousFrequency = 104.5;
AnalogSmooth as = AnalogSmooth(100);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

void setup() {
  radio.setMonoReception();
  radio.setStereoNoiseCancellingOn();
  analogWrite(6, contrast);
  lcd.begin(16, 2);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop() {
  //Commented code is for POTENTIOMETER MODE (DEBUG/LOW REMOTE BATTERY)
  //for (int i = 0; i < 30; i++) {
  //val = val + as.smooth(analogRead(analogPin));
  //delay(1);
  //}

  if (irrecv.decode(&results)) {
    switch (results.value) {
      case 0xFFA25D: //Keypad button "2"
        frequency = frequency - 0.1;
    }
    switch (results.value) {
      case 0xFFE21D: //Keypad button "2"
        frequency = frequency + 0.1;
    }
    switch (results.value) {
      case 0xFF22DD: //Keypad button "2"
        frequency = frequency - 1;
    }
    switch (results.value) {
      case 0xFF02FD: //Keypad button "2"
        frequency = frequency + 1;
    }
    irrecv.resume();
  }
  
  //val = val / 30;
  //frequencyInt = map(val, 2, 1014, 8700, 10700);
  //float frequency = frequencyInt / 100.0f;

  if (frequency != previousFrequency) {
    lcd.clear();
    radio.selectFrequency(frequency);
    printFrequency(frequency);
    previousFrequency = frequency;
  }

  //lcd.clear();
  //printFrequency(frequency);
  if (frequency > 104.3 && frequency < 104.7) {
    lcd.setCursor(0, 1);
    lcd.print("CHUM");
  }

  if (frequency > 99.8 && frequency < 100.1) {
    lcd.setCursor(0, 1);
    lcd.print("VIRGIN");
  }

  if (frequency > 98.0 && frequency < 98.2) {
    lcd.setCursor(0, 1);
    lcd.print("CHFI");
  }

  delay(20);
  val = 0;

  Serial.println(frequency);
}


void printFrequency(float frequency) {
  String frequencyString = String(frequency, 1);
  lcd.print(frequencyString + " FM");
}
