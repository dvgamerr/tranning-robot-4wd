#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 15, 2);

#define echoPin A1
#define trigPin A0
#define servoPin 11
#define RATE_SONIC 58.65
#define DELAY_SONIC 250
#define SERVO_CENTOR 1880
int maxRange = 16;
int minRange = 2;
unsigned long duration, distance;

unsigned long elapsed = 0;

int myangle = 60;

void servopulse(int servopin, int myangle) //defining a function of pulse
{
  const int adjust = 0;
  int pulsewidth = ((myangle + adjust) * 11) + 500; //converting angle into pulse width value at 500-2480
  digitalWrite(servopin, HIGH); //increasing the level of motor interface to upmost
  delayMicroseconds(pulsewidth); //delaying microsecond of pulse width value
  digitalWrite(servopin, LOW); //decreasing the level of motor interface to the least
  delay(20 - pulsewidth / 1000);
  Serial.println((String)"delay-" + (20 - pulsewidth / 1000));
}
void Set_servopulse(int set_val)
{
  for (int i = 0; i <= 20; i++) //giving motor enough time to turn to assigning point
    Serial.println((String)"loop-"+i);
    servopulse(servoPin, set_val); //invokimg pulse function
}

void setup() {
  Serial.begin(9600);
  pinMode(servoPin, OUTPUT);
  led_begin();
  init_ultrasonic();
}

bool onetime = false;
void loop() {
  ultrasonic();
  unsigned long current = millis();
  if (current - elapsed > DELAY_SONIC) {
    ultrasonic();
    elapsed = millis();
  }
}

unsigned long se = 0;
void setServo(int angle) {
  bool toggle = false;
  unsigned long sc = millis();
  if (se == 0) se = millis();
  if (sc - se < 200) {
 
  }
}

void servo(int angle) {
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(SERVO_CENTOR);
  digitalWrite(servoPin, LOW);
  delay(50);
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

