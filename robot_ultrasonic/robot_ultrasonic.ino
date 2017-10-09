#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 15, 2);

#define echoPin A3
#define trigPin A2
#define servoPin 3
#define RATE_SONIC 58.65
#define DELAY_SONIC 50
#define SERVO_CENTOR 1880
int maxRange = 21;
int minRange = 2;
unsigned long duration, distance;

unsigned long elapsed = 0;

byte myangle = 360;
byte angle_set = 0;
void pulseServo(int pin, int pulsewidth) //defining a function of pulse
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulsewidth); // delaying microsecond of pulse width value 500 - 2480
  digitalWrite(pin, LOW);
  delay(32 - pulsewidth / 800);
}
void setServo(int angle) {
  if (angle == myangle && angle_set < 3) {
    double adjust;
    Serial.print((String)"Angle: " + angle);
    bool minus = false;
    if (angle > 90) angle = 90;
    if (angle < -90) angle = -90;
    if (angle < 0) {
      adjust = 7.45;
      angle = angle * -1;
      minus = true;
    } else {
      adjust = 8.25;
    }
    
    double pulsewidth = angle * adjust;
    if (!minus) {
      pulsewidth = SERVO_CENTOR - pulsewidth;
    } else {
      pulsewidth = SERVO_CENTOR + pulsewidth;
    }
    Serial.println((String)" -- " + pulsewidth);
    for (int i = 0; i < 5; i++) pulseServo(servoPin, (int)pulsewidth);
    angle_set++;
  } else if (angle != myangle) {
    myangle = angle;
    angle_set = 0;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(servoPin, OUTPUT);
  led_begin();
  init_ultrasonic();
}

bool onetime = false;
void loop() {
  unsigned long current = millis();
  ultrasonic();
  if (current - elapsed > DELAY_SONIC) {
    setServo(0);
    elapsed = millis();
  }
}








void led_begin() {
  lcd.init();
  lcd.backlight();
}

void init_ultrasonic() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void ultrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);

  duration = duration / RATE_SONIC;
  lcd.setCursor(0, 1);
  lcd.print("Range:");
  if (duration > minRange && duration < maxRange) {
    lcd.setCursor(7, 1);
    lcd.print((String)(roundf(duration * 100) / 100) + "  ");
  } else {
    lcd.setCursor(7, 1);
    lcd.print("0      ");

  }
}

