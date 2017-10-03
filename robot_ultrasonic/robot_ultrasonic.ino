#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,15,2);

#define echoPin 12
#define trigPin 11
int maxRange = 12;
int minRange = 1;
long duration, distance;

const int sensorPinA = 4;
const int sensorPinB = 5;
const int sensorPinC = 6;

unsigned long elapsed = 0;

void setup() {
  Serial.begin(9600);
  led_begin();
  init_ultrasonic();
//  pinMode(ledPinA, OUTPUT);
//  pinMode(ledPinB, OUTPUT);
//  pinMode(ledPinC, OUTPUT);
  
  pinMode(sensorPinA, INPUT);
  pinMode(sensorPinB, INPUT);
  pinMode(sensorPinC, INPUT);
}

void loop() {
  ultrasonic();
  
  unsigned long current = millis();
  if (current - elapsed > 100) {
    ultrasonic();
    int sensorStateA = digitalRead(sensorPinA);
    int sensorStateB = digitalRead(sensorPinB);
    int sensorStateC = digitalRead(sensorPinC);
  
    lcd.setCursor(0,0);
    lcd.print("L:");
    lcd.setCursor(2,0);
    lcd.print(sensorStateC);
    lcd.setCursor(4,0);
    lcd.print("- C:");
    lcd.setCursor(8,0);
    lcd.print(sensorStateB);
    lcd.setCursor(10,0);
    lcd.print("- R:");
    lcd.setCursor(14,0);
    lcd.print(sensorStateA);
    elapsed = millis();
  }
}

void led_begin() {
  lcd.init();
  lcd.backlight();
}

void init_ultrasonic() {
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
}

void ultrasonic() {
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin,HIGH);

  duration = duration / 58.2;
  lcd.setCursor(0,1);
  lcd.print("D:");
  Serial.print("duration: ");
  Serial.println(duration);
  if (duration > minRange && duration < maxRange) {
    lcd.setCursor(2,1);
    lcd.print(duration);
  } else {
    lcd.setCursor(2,1);
    lcd.print("0");
    
  }
}

